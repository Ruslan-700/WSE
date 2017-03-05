#include <fstream>
#include <algorithm>
#include <regex>
#include "WSELuaOperationsContext.h"
#include "WSE.h"
#include "WSEScriptingContext.h"

enum lArgT
{
	lNum,
	lStr,
	lFunc
};

void gPrint(const char *msg)
{
#if defined WARBAND
	warband->window_manager.display_message(msg, 0xFFFF5555, 0);
#endif

	warband->log_stream.write_c_str(msg);
	warband->log_stream.write_c_str("\n");
}

void gPrint(const std::string &s)
{
	gPrint(s.c_str());
}

void gPrintf(const char *format, ...)
{
	char buf[1000];
	va_list args;

	va_start(args, format);
	vsprintf_s(buf, format, args);
	va_end(args);

	gPrint(buf);
}

void gPrintf(const std::string &format, ...)
{
	va_list args;

	va_start(args, format);
	gPrintf(format.c_str(), args);
	va_end(args);
}

bool fileExists(const std::string& name) {
	std::ifstream f(name.c_str());
	return f.good();
}

void removeChar(std::string &str, char c)
{
	str.erase(std::remove(str.begin(), str.end(), c), str.end());
}

void delBlank(std::string &str)
{
	removeChar(str, ' ');
	removeChar(str, '\t');
}

void discardComment(std::string &str)
{
	size_t n = str.find('#');	
	
	if (n != std::string::npos)
	{
		str.erase(n);
	}
}

template<typename Out>
void split(const std::string &s, char delim, bool skipEmpty, Out result)
{
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim))
	{
		if (!skipEmpty || item.length() > 0)
			*(result++) = item;
	}
}

std::vector<std::string> split(const std::string &s, char delim, bool skipEmpty)
{
	std::vector<std::string> elems;
	split(s, delim, skipEmpty, std::back_inserter(elems));
	return elems;
}

size_t countChar(const std::string &s, char c)
{
	return std::count(s.begin(), s.end(), c);
}

std::string getLuaScriptDir()
{
	std::string dir = "Modules\\";
	dir += warband->cur_module_name.c_str();
	dir += "\\lua\\";

	return dir;
}

inline int checkLArgs(lua_State *L, int minCount, int maxCount, ...)
{
	int numArgs = lua_gettop(L);

	if (numArgs < minCount || numArgs > maxCount)
	{
		luaL_error(L, "invalid arg count: %d - min is %d, max is %d", numArgs, minCount, maxCount);
	}

	va_list args;
	va_start(args, maxCount);

	for (int i = 1; i <= maxCount && i <= numArgs; i++)
	{
		switch (va_arg(args, lArgT))
		{
		case lArgT::lNum:{
				if (!lua_isnumber(L, i))
				{
					luaL_error(L, "arg %i must be a number", i);
				}
				break;
			}
			case lArgT::lStr:{
				if (!lua_isstring(L, i))
				{
					luaL_error(L, "arg %i must be a string", i);
				}
				break;
			}
			case lArgT::lFunc:{
				if (!lua_isfunction(L, i))
				{
					luaL_error(L, "arg %i must be a function", i);
				}
				break;
			}
		}
	}
	va_end(args);

	return numArgs;
}

inline void setOperandToLocalVar(__int64 &operand, int localsIndex)
{
	operand = ((__int64)17 << 56) + localsIndex;
}

int getTemplateNo(const char *id)
{
	for (int i = 0; i < warband->num_mission_templates; i++)
		if (warband->mission_templates[i].id == id)
			return i;

	return -1;
}


static int lGameExecOperationHandler(lua_State *L)
{
	int numLArgs = lua_gettop(L);

	if (numLArgs == 0)
		luaL_error(L, "need operation identifier");
	else if (numLArgs > 17)
		luaL_error(L, "operand count can't be > 16");

	if (!lua_isstring(L, 1))
		luaL_error(L, "invalid operation identifier");

	std::string opName(lua_tostring(L, 1));

	auto opEntry = WSE->LuaOperations.operationMap.find(opName);

	if (opEntry == WSE->LuaOperations.operationMap.end())
		luaL_error(L, "undefined operation: [%s]", opName.c_str());

	gameOperation op = opEntry->second;

	wb::operation wop;
	wop.opcode = op.opcode;
	wop.num_operands = numLArgs - 1;
	wop.end_statement = -1;

	__int64 locals[1];

	int curStrReg = NUM_REGISTERS;

	int curLArgIndex = 2;
	int curOperandIndex = 0;

	if (op.flags & WSEOperationType::LhsOperation)
	{
		locals[0] = lua_tointeger(L, curLArgIndex++);
		setOperandToLocalVar(wop.operands[curOperandIndex++], 0);
	}

	while (curLArgIndex <= numLArgs)
	{
		if (lua_isnumber(L, curLArgIndex))
			wop.operands[curOperandIndex] = lua_tointeger(L, curLArgIndex);
		else if (lua_isstring(L, curLArgIndex))
		{
			warband->basic_game.string_registers[--curStrReg] = lua_tostring(L, curLArgIndex);
			wop.operands[curOperandIndex] = curStrReg;
		}
		else
			luaL_error(L, "invalid operand #%i", curOperandIndex);

		curOperandIndex++;
		curLArgIndex++;
	}

	int e = 0;

	bool b = wop.execute(locals, 0, e);

	int retCount = 1;
	if (op.flags & WSEOperationType::CfOperation)
	{
		lua_pushboolean(L, b);
		retCount++;
	}

	if (op.flags & WSEOperationType::LhsOperation)
	{
		lua_pushinteger(L, (lua_Integer)locals[0]);
		retCount++;
	}

	lua_pushinteger(L, e);

	return retCount;

	/*lua_newtable(L);

	lua_pushinteger(L, e);
	lua_setfield(L, -2, "error");

	if (op.flags & WSEOperationType::LhsOperation)
	{
		lua_pushinteger(L, (lua_Integer)locals[0]);
		lua_setfield(L, -2, "result");
	}

	if (op.flags & WSEOperationType::CfOperation)
	{
		lua_pushboolean(L, b);
		lua_setfield(L, -2, "nofail");
	}

	return 1;*/
}

static int lGetRegHandler(lua_State *L)
{
	checkLArgs(L, 2, 2, lNum, lNum);
	/*if (lua_gettop(L) != 2)
		luaL_error(L, "invalid arg count");

	if (!lua_isnumber(L, 1) || !lua_isnumber(L, 2))
		luaL_error(L, "arg is not number");*/

	int typeId = lua_tointeger(L, 1);
	int index = lua_tointeger(L, 2);

	if (index < 0 || index >= NUM_REGISTERS)
		luaL_error(L, "index out of range");

	if (typeId == 0)
		lua_pushinteger(L, (lua_Integer)warband->basic_game.registers[index]);
	else if (typeId == 1)
		lua_pushstring(L, warband->basic_game.string_registers[index]);

	return 1;
}

static int lSetRegHandler(lua_State *L)
{
	if (lua_gettop(L) != 3)
		luaL_error(L, "invalid arg count");

	if (!lua_isnumber(L, 1) || !lua_isnumber(L, 2))
		luaL_error(L, "arg is not number");

	int typeId = lua_tointeger(L, 1);
	int index = lua_tointeger(L, 2);

	if (index < 0 || index >= NUM_REGISTERS)
		luaL_error(L, "index out of range");

	if (typeId == 0)
	{
		if (lua_isnumber(L, 3))
			warband->basic_game.registers[index] = lua_tointeger(L, 3);
		else
			luaL_error(L, "val is not number");

	}
	else if (typeId == 1)
	{
		if (lua_isstring(L, 3))
			warband->basic_game.string_registers[index] = lua_tostring(L, 3);
		else
			luaL_error(L, "val is not string");
	}

	return 0;
}

static int lGetScriptNo(lua_State *L)
{
	checkLArgs(L, 1, 1, lStr);

	const char *scriptId = lua_tostring(L, 1);

	for (int i = 0; i < warband->script_manager.num_scripts; i++)
	{
		if (warband->script_manager.scripts[i].id == scriptId)
		{
			lua_pushinteger(L, i);
			return 1;
		}
	}

	lua_pushnil(L);
	return 1;
}

static int lGetCurTemplateId(lua_State *L)
{
	lua_pushstring(L, warband->mission_templates[warband->cur_mission->cur_mission_template_no].id);
	return 1;
}

static int lAddTrigger(lua_State *L)
{
	int numArgs = checkLArgs(L, 5, 6, lStr, lNum, lNum, lNum, lFunc, lFunc);

	const char *tId = lua_tostring(L, 1);

	wb::trigger newT;

	newT.check_interval = (float)lua_tonumber(L, 2);
	newT.delay_interval = (float)lua_tonumber(L, 3);
	newT.rearm_interval = (float)lua_tonumber(L, 4);

	newT.status = wb::trigger_status::ts_ready;
	newT.check_interval_timer = rgl::timer(2);
	newT.delay_interval_timer = rgl::timer(2);
	newT.rearm_interval_timer = rgl::timer(2);

	newT.conditions.num_operations = 1;
	newT.conditions.operations = rgl::_new<wb::operation>(1);

	newT.conditions.operations[0].opcode = WSE->LuaOperations.callTriggerOpcode;
	newT.conditions.operations[0].num_operands = 1;

	if (numArgs == 6)
	{
		newT.consequences.num_operations = 1;
		newT.consequences.operations = rgl::_new<wb::operation>(1);

		newT.consequences.operations[0].opcode = WSE->LuaOperations.callTriggerOpcode;
		newT.consequences.operations[0].num_operands = 1;

		newT.consequences.operations[0].operands[0] = luaL_ref(L, LUA_REGISTRYINDEX);
	}

	newT.conditions.operations[0].operands[0] = luaL_ref(L, LUA_REGISTRYINDEX);

	int tNo = getTemplateNo(tId);

	if (tNo < 0)
		luaL_error(WSE->LuaOperations.luaState, "invalid template id: %s", tId);
	else
		warband->mission_templates[tNo].addTrigger(newT, tNo);

	return 0;
}


int opGetTop(WSELuaOperationsContext *context)
{
	return lua_gettop(context->luaState);
}

int opToInt(WSELuaOperationsContext *context)
{
	int index;
	context->ExtractValue(index);

	return lua_tointeger(context->luaState, index);
}

void opToStr(WSELuaOperationsContext *context)
{
	int index, sreg;
	context->ExtractValue(index);
	context->ExtractRegister(sreg);

	const char *str = lua_tostring(context->luaState, index);

	if (str == NULL)
		context->ScriptError("value at index %i is not a valid string", index);
	else
		warband->basic_game.string_registers[sreg] = str;
}

void opPushInt(WSELuaOperationsContext *context)
{
	int val;
	context->ExtractValue(val);

	lua_pushinteger(context->luaState, val);
}

void opPushStr(WSELuaOperationsContext *context)
{
	std::string val;
	context->ExtractString(val);

	lua_pushstring(context->luaState, val.c_str());
}

void opPop(WSELuaOperationsContext *context)
{
	int n;
	context->ExtractValue(n);

	lua_pop(context->luaState, n);
}

void opCall(WSELuaOperationsContext *context)
{
	std::string funcName;
	int numArgs;

	context->ExtractString(funcName);
	context->ExtractValue(numArgs);

	int stackSize = lua_gettop(context->luaState);

	if (stackSize < numArgs)
		context->ScriptError("not enough arguments on stack");

	lua_getglobal(context->luaState, funcName.c_str());

	if (numArgs)
		lua_insert(context->luaState, stackSize - numArgs + 1);

	if (lua_pcall(context->luaState, numArgs, LUA_MULTRET, 0))
		context->printLastError();
}

bool opTriggerCallback(WSELuaOperationsContext *context)
{
	int ref;
	context->ExtractValue(ref);

	lua_rawgeti(context->luaState, LUA_REGISTRYINDEX, ref);

	if (lua_pcall(context->luaState, 0, 1, 0))
	{
		context->printLastError();
		return false;
	}

	if (lua_gettop(context->luaState) != 1)
	{
		gPrint("Lua warning: callback needs to return true or false");
		return false;
	}

	int b = lua_toboolean(context->luaState, 1);

	return b != 0 ? true : false;
}


WSELuaOperationsContext::WSELuaOperationsContext() : WSEOperationContext("array", 5100, 5199)
{
}

void WSELuaOperationsContext::OnLoad()
{
	//RegisterOperation("testtest", test, Both, None, 0, 0,
	//	"<0> op", "test");

	RegisterOperation("lua_get_top", opGetTop, Both, Lhs, 1, 1,
		"Stores the index of the top element in the lua stack into <0>. The result also is equal to the number of elements in the stack.",
		"destination");

	RegisterOperation("lua_to_int", opToInt, Both, Lhs, 2, 2,
		"Retrieves the value at <1> from the lua stack and stores it in <0>",
		"destination", "index");

	RegisterOperation("lua_to_str", opToStr, Both, None, 2, 2,
		"Retrieves the string at <1> from the lua stack and stores it in <0>",
		"string_register", "index");

	RegisterOperation("lua_push_int", opPushInt, Both, None, 1, 1,
		"Pushes <0> onto the lua stack.",
		"value");

	RegisterOperation("lua_push_str", opPushStr, Both, None, 1, 1,
		"Pushes <0> onto the lua stack.",
		"string_1");

	RegisterOperation("lua_pop", opPop, Both, None, 1, 1,
		"Pops <0> values from the lua stack.",
		"value");

	RegisterOperation("lua_call", opCall, Both, None, 2, 2,
		"Calls the lua function with name <0>, using the lua stack to pass <1> arguments and to return values.",
		"func_name", "num_args");
	
	callTriggerOpcode = getOpcodeRangeCur();
	RegisterOperation("lua_triggerCallback", opTriggerCallback, Both, Cf, 1, 1,
		"Calls the lua trigger callback with <0>. This operation is utilized internally and should not be used, unless you know what you are doing.",
		"reference");

	initLua();
}

void WSELuaOperationsContext::OnUnload()
{
	lua_close(luaState);
}

void WSELuaOperationsContext::printLastError()
{
	const char* msg = lua_tostring(luaState, -1);
	gPrintf("Lua error: %s", msg);
	lua_pop(luaState, 1);
}

inline void WSELuaOperationsContext::initLua()
{
	luaState = luaL_newstate();
	luaL_openlibs(luaState);

	loadOperations();
	initLGameTable();
	doMainScript();
}

void WSELuaOperationsContext::applyFlagListToOperationMap(std::unordered_map<std::string, std::vector<std::string>*> &flagLists, std::string listName, unsigned short flag, std::string opFile)
{
	auto l = flagLists.find(listName);
	if (l != flagLists.end())
	{
		for (size_t i = 0; i < l->second->size(); i++)
		{
			std::string curKey = l->second->at(i);
			auto op = operationMap.find(curKey);

			if (op == operationMap.end())
				gPrintf("WSELuaOperationsContext: Warning reading %s, trying to set flag %s for non-existing operation [%s]", opFile.c_str(), listName.c_str(), curKey.c_str());
			else
				op->second.flags |= flag;
		}
	}
}

inline void WSELuaOperationsContext::loadOperations()
{
	std::string opFile = getLuaScriptDir() + "header_operations.py";
	if (!fileExists(opFile))
		return;

	std::ifstream opStream(opFile);
	std::string curLine;
	int curLineNum = 0;

	std::smatch curMatches;

	std::regex opRegEx( R"((\w+)=((0x)?\d+).*)" );
	std::regex opOrRegEx( R"((\w+)=(\w+)\|(\w+).*)" );
	std::regex listStartRegEx( R"((\w+)(\+)?=\[((?:\w+,)*(?:\w+)?)(\]?).*)" );

	std::unordered_map<std::string, std::vector<std::string>*> flagLists;

	while (std::getline(opStream, curLine))
	{
		curLineNum++;
		delBlank(curLine);

		if (curLine.length() == 0 || curLine[0] == '#')
			continue;
		
		if (std::regex_match(curLine, curMatches, opRegEx))
		{

			gameOperation newOp;
			newOp.flags = 0;

			if (curMatches.str(3).length())
				newOp.opcode = std::strtoul(curMatches.str(2).c_str(), 0, 16);
			else
				newOp.opcode = std::strtoul(curMatches.str(2).c_str(), 0, 10);

			operationMap[curMatches.str(1)] = newOp;

		}
		else if (std::regex_match(curLine, curMatches, listStartRegEx))
		{
			std::string listName = curMatches.str(1);
			bool add = curMatches.str(2) == "+";
			std::string curKeysStr = curMatches.str(3);
			bool end = curMatches.str(4) == "]";

			auto l = flagLists.find(listName);

			if (!add)
			{
				if (l != flagLists.end())
					delete l->second;

				flagLists[listName] = new std::vector<std::string>();
			}
			else
			{
				if (l == flagLists.end()) 
				{
					gPrintf("WSELuaOperationsContext: Error reading %s, line %i: trying to extend a non-existing list", opFile.c_str(), curLineNum);
					continue;
				}
			}

			if (curKeysStr.length())
			{
				std::vector<std::string> curKeys = split(curKeysStr, ',', true);

				for (size_t i = 0; i < curKeys.size(); i++)
					flagLists[listName]->push_back(curKeys[i]);
			}

			curKeysStr = "";
			while (!end && std::getline(opStream, curLine))
			{
				curLineNum++;
				delBlank(curLine);
				discardComment(curLine);

				size_t endPos = curLine.find(']');

				if (endPos != std::string::npos)
				{
					end = true;
					curLine.erase(endPos);
				}

				curKeysStr += curLine;
			}

			if (curKeysStr.length()) 
			{
				std::vector<std::string> curKeys = split(curKeysStr, ',', true);

				for (size_t i = 0; i < curKeys.size(); i++)
					flagLists[listName]->push_back(curKeys[i]);
			}			
		}
		else if (std::regex_match(curLine, curMatches, opOrRegEx))
		{
			auto op1 = operationMap.find(curMatches.str(2));
			auto op2 = operationMap.find(curMatches.str(3));

			if (op1 == operationMap.end() || op2 == operationMap.end())
			{
				gPrintf("WSELuaOperationsContext: Error reading %s, line %i: undefined value", opFile.c_str(), curLineNum);
			}
			else
			{
				gameOperation newOp;
				newOp.opcode = op1->second.opcode | op2->second.opcode;
				newOp.flags = 0;

				operationMap[curMatches.str(1)] = newOp;
			}
		}
		else
		{
			gPrintf("WSELuaOperationsContext: Warning reading %s, could not process line %i", opFile.c_str(), curLineNum);
		}
	}

	applyFlagListToOperationMap(flagLists, "lhs_operations", WSEOperationFlags::Lhs, opFile);
	applyFlagListToOperationMap(flagLists, "global_lhs_operations", WSEOperationFlags::Lhs, opFile);
	applyFlagListToOperationMap(flagLists, "can_fail_operations", WSEOperationFlags::Cf, opFile);

	auto l = flagLists.begin();
	while (l != flagLists.end())
	{
		delete l->second;
		l++;
	}
}

inline void WSELuaOperationsContext::initLGameTable()
{
	lua_newtable(luaState);
	
	lua_pushcfunction(luaState, lGameExecOperationHandler);
	lua_setfield(luaState, -2, "execOperation");

	lua_pushcfunction(luaState, lGetRegHandler);
	lua_setfield(luaState, -2, "getReg");

	lua_pushcfunction(luaState, lSetRegHandler);
	lua_setfield(luaState, -2, "setReg");

	lua_pushcfunction(luaState, lGetScriptNo);
	lua_setfield(luaState, -2, "getScriptNo");

	lua_pushcfunction(luaState, lGetCurTemplateId);
	lua_setfield(luaState, -2, "getCurTemplateId");

	lua_pushcfunction(luaState, lAddTrigger);
	lua_setfield(luaState, -2, "addTrigger");

	lua_setglobal(luaState, "game");

	const char *s =
		#include "WSELuaOperationsLuaCode.txt"
		;

		/*"game.mt = {}\n"
		"game.mt.__index = function(table, key)\n"
			"return function(...)\n"
				"return game.execOperation(key,...)\n"
			"end\n"
		"end\n"
		"setmetatable(game, game.mt)\n";*/

	if (luaL_dostring(luaState, s))
		printLastError();
}

inline void WSELuaOperationsContext::doMainScript()
{
	std::string mainFile = getLuaScriptDir() + "main.lua";

	if (fileExists(mainFile))
	{
		int result = luaL_loadfile(luaState, mainFile.c_str());

		if (result)
		{
			printLastError();
		}
		else
		{
			result = lua_pcall(luaState, 0, LUA_MULTRET, 0);
			if (result)
				printLastError();
		}
	}
}
