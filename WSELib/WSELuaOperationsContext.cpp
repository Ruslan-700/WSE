#include <regex>
#include "WSELuaOperationsContext.h"
#include "WSE.h"
#include "WSEScriptingContext.h"

#include "WSELuaOperationsHelpers.h"
#include "WSELuaOperationsLuaCallbacks.h"

#include "lanes.h"
#include <Windows.h>
#include "WSELib.rc.h"

/************************/
/*    MS operations    */
/************************/

int opGetTop(WSELuaOperationsContext *context)
{
	return lua_gettop(context->luaState);
}

void opSetTop(WSELuaOperationsContext *context)
{
	int index;
	context->ExtractValue(index);

	lua_settop(context->luaState, index);
}

void opInsert(WSELuaOperationsContext *context)
{
	int index;
	context->ExtractValue(index);

	checkStackIndex(context, index);

	lua_insert(context->luaState, index);
}

void opRemove(WSELuaOperationsContext *context)
{
	int index;
	context->ExtractValue(index);

	checkStackIndex(context, index);

	lua_remove(context->luaState, index);
}

void opPop(WSELuaOperationsContext *context)
{
	int n;
	context->ExtractValue(n);

	lua_pop(context->luaState, n);
}

int opToInt(WSELuaOperationsContext *context)
{
	int index;
	context->ExtractValue(index);

	checkStackIndex(context, index);
	
	return lua_tointeger(context->luaState, index);
}

void opToStr(WSELuaOperationsContext *context)
{
	int index, sreg;
	context->ExtractRegister(sreg);
	context->ExtractValue(index);

	checkStackIndex(context, index);

	const char *str = lua_tostring(context->luaState, index);

	if (str == NULL)
		context->ScriptError("value at index %i is not a valid string", index);
	else
		warband->basic_game.string_registers[sreg] = str;
}

void opToPos(WSELuaOperationsContext *context)
{
	int index, preg;
	context->ExtractRegister(preg);
	context->ExtractValue(index);

	checkStackIndex(context, index);

	if (!lIsPos(context->luaState, index))
		context->ScriptError("value at index %i is not a valid position", index);
	else
		warband->basic_game.position_registers[preg] = lToPos(context->luaState, index);
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

void opPushPos(WSELuaOperationsContext *context)
{
	int preg;
	context->ExtractRegister(preg);

	lPushPos(context->luaState, warband->basic_game.position_registers[preg]);
}

int opGetType(WSELuaOperationsContext *context)
{
	int index;
	context->ExtractValue(index);

	checkStackIndex(context, index);

	return lua_type(context->luaState, index);
}

bool opCall(WSELuaOperationsContext *context)
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

	context->dontFailMsCall = true;

	if (lua_pcall(context->luaState, numArgs, LUA_MULTRET, 0))
		printLastLuaError(context->luaState);

	return context->dontFailMsCall;
}

bool opTriggerCallback(WSELuaOperationsContext *context)
{
	int ref;
	context->ExtractValue(ref);

	lua_rawgeti(context->luaState, LUA_REGISTRYINDEX, ref);

	if (lua_pcall(context->luaState, 0, 1, 0))
	{
		printLastLuaError(context->luaState);
		return false;
	}

	//printStack(context->luaState);

	if (lua_gettop(context->luaState) != 1) //TODO
	{
		gPrint("Lua warning: callback needs to return true or false");
		return false;
	}

	int b = lua_toboolean(context->luaState, 1);

	lua_settop(context->luaState, 0);

	return b != 0 ? true : false;
}

/************************/


/***********************/
/* init main lua state */
/***********************/

inline void addGameConstantsToLState(lua_State *L)
{
	std::vector<gameConstTable> &constTables = WSE->LuaOperations.gameConstTables;

	lua_newtable(L);

	for (size_t i = 0; i < constTables.size(); i++)
	{
		lua_newtable(L);
		for (size_t j = 0; j < constTables[i].constants.size(); j++)
		{
			lua_pushnumber(L, (lua_Number)constTables[i].constants[j].val);
			lua_setfield(L, -2, constTables[i].constants[j].name.c_str());
		}
		lua_setfield(L, -2, constTables[i].name.c_str());
	}

	lua_setfield(L, -2, "const");
}

void initLGameTable(lua_State *L)
{
	lua_newtable(L);

	lua_pushcfunction(L, lGameExecOperationHandler);
	lua_setfield(L, -2, "execOperation");

	lua_pushcfunction(L, lGetRegHandler);
	lua_setfield(L, -2, "getReg");

	lua_pushcfunction(L, lSetRegHandler);
	lua_setfield(L, -2, "setReg");

	lua_pushcfunction(L, lGetScriptNo);
	lua_setfield(L, -2, "getScriptNo");

	lua_pushcfunction(L, lGetCurTemplateId);
	lua_setfield(L, -2, "getCurTemplateId");

	lua_pushcfunction(L, lAddTrigger);
	lua_setfield(L, -2, "addTrigger");

	lua_pushcfunction(L, lRemoveTrigger);
	lua_setfield(L, -2, "removeTrigger");

	lua_pushcfunction(L, lAddItemTrigger);
	lua_setfield(L, -2, "addItemTrigger");

#if defined WARBAND
	lua_pushcfunction(L, lAddPrsnt);
	lua_setfield(L, -2, "addPrsnt");

	lua_pushcfunction(L, lRemovePrsnt);
	lua_setfield(L, -2, "removePrsnt");
#endif

	lua_pushcfunction(L, lPartiesIterInit);
	lua_setfield(L, -2, "partiesI");

	lua_pushcfunction(L, lAgentsIterInit);
	lua_setfield(L, -2, "agentsI");

	lua_pushcfunction(L, lPropInstIterInit);
	lua_setfield(L, -2, "propInstI");

	lua_pushcfunction(L, lPlayersIterInit);
	lua_setfield(L, -2, "playersI");

	lua_pushcfunction(L, lHookOperation);
	lua_setfield(L, -2, "hookOperation");

	lua_pushcfunction(L, lUnhookOperation);
	lua_setfield(L, -2, "unhookOperation");

	lua_pushcfunction(L, lFailMsCall);
	lua_setfield(L, -2,  "fail");

	addGameConstantsToLState(L);

	lua_setglobal(L, "game");

	lua_pushcfunction(L, lPrint);
	lua_setglobal(L, "_print");

	lua_pushcfunction(L, lGetTime);
	lua_setglobal(L, "getTime");

	const char *globals =
		#include "LuaGlobals.txt"
		;

	if (luaL_dostring(L, globals))
		printLastLuaError(L, "LuaGlobals");
}

int loadLanesLua(lua_State *L)
{
#if defined WARBAND_DEDICATED
	HMODULE handle = GetModuleHandle("wselibdedicated.dll");
#elif defined WARBAND
	HMODULE handle = GetModuleHandle("wselib.dll");
#elif defined WARBAND_NOSSE
	HMODULE handle = GetModuleHandle("wselibnosse.dll");
#endif

	HRSRC rc = FindResource(handle, MAKEINTRESOURCE(IDR_LUA_LANES),
		MAKEINTRESOURCE(TEXTFILE));
	HGLOBAL rcData = LoadResource(handle, rc);
	DWORD size = SizeofResource(handle, rc);
	const char *data = static_cast<const char*>(::LockResource(rcData));

	char *lanes = (char*)malloc(size + 1);
	memcpy_s(lanes, size + 1, data, size);
	lanes[size] = '\0';

	if (luaL_dostring(L, lanes))
		printLastLuaError(L, "lanes");

	free(lanes);
	return 1;
}

void initLaneState(lua_State *L)
{
	lua_setUserDir(L, getLuaScriptDir().c_str());
	initLGameTable(L);
}

/***************************/


WSELuaOperationsContext::WSELuaOperationsContext() : WSEOperationContext("lua", 5100, 5199)
{
	tStart = std::chrono::steady_clock::now();

	for (size_t i = 0; i < WSE_MAX_NUM_OPERATIONS; i++)
		operationHookLuaRefs[i] = LUA_NOREF;
}

void WSELuaOperationsContext::OnLoad()
{
	RegisterOperation("lua_get_top", opGetTop, Both, Lhs, 1, 1,
		"Stores the index of the top element in the lua stack into <0>. The result also is equal to the number of elements in the stack.",
		"destination");

	RegisterOperation("lua_set_top", opSetTop, Both, None, 1, 1,
		"Sets the top of the stack to <0>. Setting it to 0 just clears the entire stack.",
		"index");

	RegisterOperation("lua_insert", opInsert, Both, None, 1, 1,
		"Moves the stacks top element into <0>, shifting up the elements above <0> to open space.",
		"index");

	RegisterOperation("lua_remove", opRemove, Both, None, 1, 1,
		"Removes the element at the given <0>, shifting down the elements above <0> to fill the gap.",
		"index");

	RegisterOperation("lua_pop", opPop, Both, None, 1, 1,
		"Pops <0> values from the lua stack.",
		"value");

	RegisterOperation("lua_to_int", opToInt, Both, Lhs, 2, 2,
		"Retrieves the value at <1> from the lua stack and stores it in <0>",
		"destination", "index");

	RegisterOperation("lua_to_str", opToStr, Both, None, 2, 2,
		"Retrieves the string at <1> from the lua stack and stores it in <0>",
		"string_register", "index");

	RegisterOperation("lua_to_pos", opToPos, Both, None, 2, 2,
		"Retrieves the position at <1> from the lua stack and stores it in <0>",
		"pos_register", "index");

	RegisterOperation("lua_push_int", opPushInt, Both, None, 1, 1,
		"Pushes <0> onto the lua stack.",
		"value");

	RegisterOperation("lua_push_str", opPushStr, Both, None, 1, 1,
		"Pushes <0> onto the lua stack.",
		"string_1");

	RegisterOperation("lua_push_pos", opPushPos, Both, None, 1, 1,
		"Pushes the position in <0> onto the lua stack.",
		"pos_register");

	RegisterOperation("lua_get_type", opGetType, Both, Lhs, 1, 1,
		"Stores the type of the value at <1> in the stack into <0>. Return types can be found in header_common(_addon).py (LUA_T*)",
		"destination", "index");

	RegisterOperation("lua_call", opCall, Both, Cf, 2, 2,
		"Calls the lua function with name <0>, using the lua stack to pass <1> arguments and to return values. The first argument is pushed first. All arguments get removed from the stack automatically. The last return value will be at the top of the stack.",
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

void WSELuaOperationsContext::OnEvent(WSEContext *sender, WSEEvent evt, void *data)
{
	WSEOperationContext::OnEvent(sender, evt, data);

	if (luaStateIsReady && evt == OnRglLogMsg)
	{
		lua_getglobal(luaState, "game");
		lua_pushstring(luaState, "OnRglLogWrite");
		lua_rawget(luaState, -2);

		if (lua_type(luaState, -1) == LUA_TFUNCTION)
		{
			rglLogWriteEventData *dt = (rglLogWriteEventData*)data;

			lua_pushstring(luaState, dt->str);
			if (lua_pcall(luaState, 1, 0, 0))
			{
#if defined WARBAND
				warband->window_manager.display_message(lua_tostring(luaState, -1), 0xFFFF5555, 0);
#else
				lua_pushvalue(luaState, -1);
				printLastLuaError(this->luaState, NULL, GetStdHandle(STD_OUTPUT_HANDLE));
#endif

				printLastLuaError(this->luaState, NULL, dt->hFile);
			}

			lua_pop(luaState, 1);
		}
		else
			lua_pop(luaState, 2);
	}
}

void WSELuaOperationsContext::hookOperation(lua_State *L, int opcode, int lRef)
{
	if (operationHookLuaRefs[opcode] != LUA_NOREF)
		unhookOperation(L, opcode);

	operationHookLuaRefs[opcode] = lRef;

	if (opcode >= WSE_FIRST_WARBAND_OPCODE && opcode <= WSE_LAST_WARBAND_OPCODE)
		WSE->Hooks.HookJumptable(this, wb::addresses::operation_Execute_jumptable, opcode - 30, LuaOperationJumptableHook);
}

bool WSELuaOperationsContext::unhookOperation(lua_State *L, int opcode)
{
	if (operationHookLuaRefs[opcode] == LUA_NOREF)
		return false;

	luaL_unref(L, LUA_REGISTRYINDEX, operationHookLuaRefs[opcode]);
	operationHookLuaRefs[opcode] = LUA_NOREF;

	if (opcode >= WSE_FIRST_WARBAND_OPCODE && opcode <= WSE_LAST_WARBAND_OPCODE)
		WSE->Hooks.UnhookJumptable(this, wb::addresses::operation_Execute_jumptable, opcode - 30);

	return true;
}

bool WSELuaOperationsContext::OnOperationExecute(int lRef, int num_operands, int *operand_types, __int64 *operand_values, bool *continue_loop, bool &setRetVal, long long &retVal)
{
	setRetVal = false;
	
	int oldTop = lua_gettop(luaState);
	lua_rawgeti(luaState, LUA_REGISTRYINDEX, lRef);

	for (int i = 0; i < num_operands; i++)
	{
		int val = (int)operand_values[i];
		int type = operand_types[i];

		if (type == 3 || type == 22) //string or quickstring
		{
			rgl::string str;
			rgl::string temp;

			warband->string_manager.get_operand_string(temp, val, type);
			warband->basic_game.parse_string(str, temp);

			lua_pushstring(luaState, str.c_str());
		}
		else
		{
			lua_pushinteger(luaState, (lua_Integer)val);
		}
	}

	if (lua_pcall(luaState, num_operands, LUA_MULTRET, 0))
	{
		printLastLuaError(luaState);
		return true;
	}

	int nResults = lua_gettop(luaState) - oldTop;

	if (nResults == 0)
		return true;
	
	if (nResults == 2)
	{
		if (lua_type(luaState, 2 + oldTop) == LUA_TBOOLEAN)
			*continue_loop = lua_toboolean(luaState, 2 + oldTop) != 0;
		else if (lua_type(luaState, 2 + oldTop) == LUA_TNUMBER)
		{
			setRetVal = true;
			retVal = (long long)lua_tointeger(luaState, 2 + oldTop);
		}
		else
		{
			gPrint("invalid return value #2 type, must be bool or num");
			lua_settop(luaState, oldTop);
			return true;
		}
	}
	else if (nResults == 3) //?
	{
		if (lua_type(luaState, 2 + oldTop) == LUA_TBOOLEAN && lua_type(luaState, 3 + oldTop) == LUA_TNUMBER)
		{
			*continue_loop = lua_toboolean(luaState, 2 + oldTop) != 0;
			setRetVal = true;
			retVal = (long long)lua_tointeger(luaState, 3 + oldTop);
		}
		else
		{
			gPrint("invalid return values #2 and #3, must be bool and num");
			lua_settop(luaState, oldTop);
			return true;
		}
	}

	bool cont = lua_toboolean(luaState, 1 + oldTop) != 0;
	lua_settop(luaState, oldTop);
	return cont;
}

void *WSELuaOperationsContext::OnOperationJumptableExecute(wb::operation *operation, int *operand_types, __int64 *operand_values, bool *continue_loop, __int64 *locals, int context_flags)
{
	int opcode = operation->opcode & 0xFFFFFFF;

	try
	{
		bool setRetVal;
		long long retVal;

		bool cont = this->OnOperationExecute(operationHookLuaRefs[opcode], operation->num_operands, operand_types, operand_values, continue_loop, setRetVal, retVal);

		if (setRetVal)
			operation->set_return_value(locals, retVal);

		if (cont)
		{
			void *addr;
			WSE->Hooks.getHookBackup(wb::addresses::operation_Execute_jumptable + (opcode - 30) * 4, 4, (unsigned char*)&addr, 4);
			return addr;
		}

		return NULL;
	}
	catch (...)
	{
	}

	return NULL;
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
	std::string opFile = getLuaScriptDir() + "msfiles\\" + "header_operations.py";
	if (!fileExists(opFile))
		return;

	std::ifstream opStream(opFile);
	std::string curLine;
	int curLineNum = 0;

	std::smatch curMatches;

	std::regex opRegEx(R"((\w+)=(((0x)[\da-fA-F]+)|(\d+)).*)");
	std::regex opOrRegEx(R"((\w+)=(\w+)\|(\w+).*)");
	std::regex listStartRegEx(R"((\w+)(\+)?=\[((?:\w+,)*(?:\w+)?)(\]?).*)");

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

			if (curMatches.str(4).length())
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

void WSELuaOperationsContext::loadGameConstants(const std::string &dir)
{
	WIN32_FIND_DATA ffd;

	HANDLE hFind = FindFirstFile((dir + "*").c_str(), &ffd);

	if (hFind == INVALID_HANDLE_VALUE)
		return;

	std::smatch curMatches;
	std::regex fnRegEx(R"(^(header|ID|module)_(\w+)\.[^\.]+$)");

	do
	{
		std::string s = ffd.cFileName;

		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (s != "." && s != "..")
				loadGameConstants(dir + s + "\\");
		}
		else if (s != "header_operations.py")
		{
			if (std::regex_match(s, curMatches, fnRegEx))
				loadGameConstantsFromFile(dir + s, gameConstTables, curMatches.str(2));
			else
			{
				size_t lastdot = s.find_last_of(".");

				loadGameConstantsFromFile(dir + s, gameConstTables, lastdot == std::string::npos ? s : s.substr(0, lastdot));
			}
		}
	} while (FindNextFile(hFind, &ffd));
	FindClose(hFind);
}

inline void WSELuaOperationsContext::initLua()
{
	luaState = luaL_newstate();
	lua_setUserDir(luaState, getLuaScriptDir().c_str());
	luaL_openlibs(luaState);

	luaopen_lanes_embedded(luaState, initLaneState, loadLanesLua);
	lua_pop(luaState, 1);

	loadOperations();
	loadGameConstants(getLuaScriptDir() + "msfiles\\");

	initLGameTable(luaState);
	doMainScript();

	luaStateIsReady = true;
}

inline void WSELuaOperationsContext::doMainScript()
{
	std::string mainFile = getLuaScriptDir() + "main.lua";

	if (fileExists(mainFile))
	{
		if (luaL_loadfile(luaState, mainFile.c_str()))
		{
			printLastLuaError(luaState);
		}
		else
		{
			if (lua_pcall(luaState, 0, 0, 0))
				printLastLuaError(luaState);
		}
	}
}
