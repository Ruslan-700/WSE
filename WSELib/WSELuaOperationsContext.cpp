#include <regex>
#include "WSELuaOperationsContext.h"
#include "WSE.h"
#include "WSEScriptingContext.h"

#include "WSELuaOperationsHelpers.h"
#include "WSELuaOperationsLuaCallbacks.h"

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

void opCall(WSELuaOperationsContext *context)//TEST
{
	std::string funcName;
	int numArgs;

	context->ExtractString(funcName);
	context->ExtractValue(numArgs);

	int stackSize = lua_gettop(context->luaState);

	if (stackSize < numArgs)
		context->ScriptError("not enough arguments on stack");

	lua_getglobal(context->luaState, "sandboxRun");
	lua_pushstring(context->luaState, funcName.c_str());

	if (numArgs)
	{
		lua_insert(context->luaState, stackSize - numArgs + 1);
		lua_insert(context->luaState, stackSize - numArgs + 1);
	}

	if (lua_pcall(context->luaState, numArgs + 1, LUA_MULTRET, 0))
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

	//printStack(context->luaState);

	if (lua_gettop(context->luaState) != 1)
	{
		gPrint("Lua warning: callback needs to return true or false");
		return false;
	}

	int b = lua_toboolean(context->luaState, 1);

	lua_settop(context->luaState, 0);

	return b != 0 ? true : false;
}


WSELuaOperationsContext::WSELuaOperationsContext() : WSEOperationContext("array", 5100, 5199)
{
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

	RegisterOperation("lua_call", opCall, Both, None, 2, 2,
		"Calls the lua function with name <0>, using the lua stack to pass <1> arguments and to return values.",
		"func_name", "num_args");
	
	callTriggerOpcode = getOpcodeRangeCur();
	RegisterOperation("lua_triggerCallback", opTriggerCallback, Both, Cf, 1, 1,
		"Calls the lua trigger callback with <0>. This operation is utilized internally and should not be used, unless you know what you are doing.",
		"reference");

	initLua();
	gIterators.reserve(3);
}

void WSELuaOperationsContext::OnUnload()
{
	lua_close(luaState);
}

void WSELuaOperationsContext::printLastError(const char *fileName)
{
	const char *msg = lua_tostring(luaState, -1);

	if (fileName)
		gPrintf("Lua error in %s: %s", fileName, msg);
	else
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

int WSELuaOperationsContext::gIteratorAdd(const gameIterator &it)
{
	size_t i = 0;
	while (i < gIterators.size())
	{
		if (!gIterators[i].valid)
		{
			gIterators[i] = it;
			return i;
		}

		i++;
	}

	if (i < maxGiterators)
	{
		gIterators.push_back(it);
		return i;
	}
	else
		return -1;

}

gameIterator *WSELuaOperationsContext::getGiterator(size_t itNo)
{
	if (itNo < 0 || itNo >= gIterators.size() || !gIterators[itNo].valid)
		return NULL;

	return &gIterators[itNo];
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

inline void WSELuaOperationsContext::initLGameTable() //rename
{
	const char *sandbox =
		#include "LuaSandbox.txt"
		;

	if (luaL_dostring(luaState, sandbox))
		printLastError("LuaSandbox");

	lua_getglobal(luaState, "sandboxInit");
	lua_pushstring(luaState, getLuaScriptDir().c_str());

	if (lua_pcall(luaState, 1, 0, 0))
		printLastError("LuaSandbox");


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

	lua_pushcfunction(luaState, lRemoveTrigger);
	lua_setfield(luaState, -2, "removeTrigger");

	lua_pushcfunction(luaState, lPartiesIterInit);
	lua_setfield(luaState, -2, "partiesIterInit");

	lua_pushcfunction(luaState, lAgentsIterInit);
	lua_setfield(luaState, -2, "agentsIterInit");

	lua_pushcfunction(luaState, lPropInstIterInit);
	lua_setfield(luaState, -2, "propInstIterInit");

	lua_pushcfunction(luaState, lPlayersIterInit);
	lua_setfield(luaState, -2, "playersIterInit");

	lua_pushcfunction(luaState, lIterNext);
	lua_setfield(luaState, -2, "iterNext");

	lua_setglobal(luaState, "game");

	const char *globals =
		#include "LuaGlobals.txt"
		;

	if (luaL_dostring(luaState, globals))
	{
		printLastError("LuaGlobals");
	}
}

inline void WSELuaOperationsContext::doMainScript()
{
	std::string mainFile = getLuaScriptDir() + "main.lua";

	if (fileExists(mainFile))
	{
		lua_getglobal(luaState, "sandboxRun");

		if (luaL_loadfile(luaState, mainFile.c_str()))
		{
			printLastError();
			lua_pop(luaState, 1);
		}
		else
		{
			if (lua_pcall(luaState, 1, 0, 0))
				printLastError();
		}
	}
}
