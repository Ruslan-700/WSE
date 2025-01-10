#include <regex>
#include <Windows.h>

#include "WSE.h"
#include "WSELib.rc.h"
#include "WSEScriptingContext.h"
#include "WSELuaOperationsContext.h"
#include "WSELua_Helpers.h"
#include "WSELua_Callbacks.h"

#include "lanes.h"


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
	//gPrintf("lua_call top1: %i", stackSize);

	if (stackSize < numArgs)
		context->ScriptError("not enough arguments on stack");

	WSE->LuaOperations.luaContext = context->GetCurrentTrigger();

	lua_getglobal(context->luaState, funcName.c_str());

	if (numArgs)
		lua_insert(context->luaState, stackSize - numArgs + 1);

	lua_pushcfunction(context->luaState, traceback);
	lua_insert(context->luaState, stackSize - numArgs + 1);

	context->lua_call_cfResults.push_back(true);

	if (lua_pcall(context->luaState, numArgs, LUA_MULTRET, stackSize - numArgs + 1))
	{
		printLastLuaError(context->luaState);
	}

	bool cf = context->lua_call_cfResults.back();
	context->lua_call_cfResults.pop_back();

	//gPrintf("lua_call top2: %i", lua_gettop(context->luaState));
	return cf;
}

bool opTriggerCallback(WSELuaOperationsContext *context)
{
	int ref, part;
	int results = 0;

	context->ExtractValue(ref);
	context->ExtractValue(part);
	context->ExtractValue(WSE->LuaOperations.luaContext);

	if (part == triggerPart::condition)
	{
		results = 1;
	}

	lua_pushcfunction(context->luaState, traceback);
	lua_rawgeti(context->luaState, LUA_REGISTRYINDEX, ref);

	if (lua_pcall(context->luaState, 0, results, -2))
	{
		printLastLuaError(context->luaState);
		return false;
	}

	if (part == triggerPart::condition)
	{
		int b = 1;

		if (lua_isboolean(context->luaState, -1))
		{
			b = lua_toboolean(context->luaState, -1);
		}
		
		lua_pop(context->luaState, 2);

		return b != 0 ? true : false;
	}

	lua_pop(context->luaState, 1);
	return true;
}

/************************/


/***********************/
/* init main lua state */
/***********************/

//This is a callback for luaJIT
//We try to restrict all IO to user or storage dir with this middleman.
#define STORAGE "%storage%"
char* sandbox_path(const char* _path)
{
	if (_path == NULL)
		return NULL;

	char* path = _strdup(_path);
	char* path_orig = path; //Might modify path pointer, but still want to free at the end

	//Pick root_dir. Magic prefix <WSE> will access storage dir
	const char* root_dir;
	if (str_starts_with(path, STORAGE, true))
	{
		root_dir = WSE->LuaOperations.storage_dir.c_str();
		path += strlen(STORAGE);
		if (str_starts_with(path, "\\")) path++;
	}
	else{
		root_dir = WSE->LuaOperations.user_dir.c_str();
	}

	int curLevel = 0;
	int points = 0;
	int others = 0;

	/*
	Abort on : or !
	Abort when going below start level with ..
	*/
	size_t i = 0;
	while (i < strlen(path))
	{
		if (path[i] == ':' || path[i] == '!')
		{
			free(path_orig);
			return NULL;
		}

		if (path[i] == '.')
			points++;
		else if (path[i] != '/' && path[i] != '\\')
			others++;
		else // '/' or '\\'
		{
			if (path[i] == '/') path[i] = '\\'; //we only want backslash

			if (others)
				curLevel++;
			else if (points >= 2)
			{
				curLevel--;
				if (curLevel < 0)
				{
					free(path_orig);
					return NULL;
				}
			}

			points = 0;
			others = 0;
		}

		i++;
	}
	//Path looks safe...

	//we skip leading ".\" so we dont have rootDir\.\path
	if (str_starts_with(path, ".\\"))
	{
		if (root_dir[strlen(root_dir) - 1] == '\\') //also good idea to make sure that root_dir ends with '\\'
		{
			path += 2;
		}
		else{
			path += 1;
		}
	}
	else if (str_starts_with(path, "\\")) //skip '\\' also
	{
		path++;
	}

	size_t spSize = strlen(root_dir) + strlen(path) + 1;
	char *safePath = (char*)malloc(spSize);

	strcpy_s(safePath, spSize, root_dir);
	strcat_s(safePath, spSize, path);

	/*FILE* f = fopen("dbg.txt", "a");
	fprintf(f, "%s , %s\n", root_dir, path);
	fclose(f);*/

	free(path_orig);

	return safePath;
}

int loadLanesLua(lua_State *L)
{
	std::string lanes = load_resource_str(MAKEINTRESOURCE(IDR_LUA_LANES));

	if (luaL_dostring(L, lanes.c_str()))
		printLastLuaError(L, "lanes");

	return 1;
}

void initLaneState(lua_State *L)
{
	lua_set_sandboxed_path_callback(L, sandbox_path);
	initLGameTable(L);
}

/***************************/
WSELuaOperationsContext::WSELuaOperationsContext() : WSEOperationContext("lua", 5100, 5199)
{
	this->tStart = std::chrono::steady_clock::now();

	for (size_t i = 0; i < WSE_MAX_NUM_OPERATIONS; i++)
	{
		this->operationHookLuaRefs[i] = LUA_NOREF;
	}
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

	RegisterOperation("lua_get_type", opGetType, Both, Lhs, 2, 2,
		"Stores the type of the value at <1> in the stack into <0>. Return types can be found in header_common(_addon).py (LUA_T*)",
		"destination", "index");

	RegisterOperation("lua_call", opCall, Both, Cf, 2, 2,
		"Calls the lua function with name <0>, using the lua stack to pass <1> arguments and to return values. The first argument is pushed first. All arguments get removed from the stack automatically. The last return value will be at the top of the stack.",
		"func_name", "num_args");

	callTriggerOpcode = getOpcodeRangeCur();
	RegisterOperation("lua_triggerCallback", opTriggerCallback, Both, Cf, 2, 3,
		"Calls the lua trigger callback with <0>. This operation is utilized internally and should not be used, unless you know what you are doing.",
		"reference", "triggerPart", "context");

	initLua();
}

void WSELuaOperationsContext::OnUnload()
{
	lua_close(luaState);
}

void WSELuaOperationsContext::OnEvent(WSEContext *sender, WSEEvent evt, void *data)
{
	WSEOperationContext::OnEvent(sender, evt, data);
	
	if (!luaStateIsReady) return;

	if (evt == WSEEvent::OnFrame)
	{
		if (lua_gettop(this->luaState) > 100)
		{
			lua_settop(this->luaState, 0);
		}
	}
	else if (evt == WSEEvent::OnChatMessageReceived)
	{
		int top = lua_gettop(luaState);
		lua_getglobal(luaState, "game");
		if (lua_isnil(luaState, -1)) { lua_pop(luaState, 1); return; };

		lua_pushstring(luaState, "OnChatMessageReceived");
		lua_rawget(luaState, -2);

		if (lua_type(luaState, -1) == LUA_TFUNCTION)
		{
			chatMessageReceivedEventData *dt = (chatMessageReceivedEventData*)data;
			lua_pushinteger(luaState, dt->player);
			lua_pushboolean(luaState, dt->team_chat);
			lua_pushstring(luaState, dt->text->c_str());

			if (lua_pcall(luaState, 3, 1, 0))
			{
				printLastLuaError(luaState);
			}
			else
			{
				int type = lua_type(luaState, -1);

				if (type == LUA_TBOOLEAN)
				{
					warband->basic_game.trigger_result = (long long)lua_toboolean(luaState, -1);
				}
				else if (type == LUA_TSTRING)
				{
					warband->basic_game.trigger_result = 0;
					warband->basic_game.result_string = rgl::string(lua_tostring(luaState, -1));
				}
				else if (type != LUA_TNIL)
				{
					luaL_error(luaState, "return value must be int, string or nil");
				}

				lua_pop(luaState, 1);
			}

			lua_pop(luaState, 1); // "game" table
		}
		else 
		{
			lua_pop(luaState, 2);
		}
	}
	else if (evt == WSEEvent::OnRglLogMsg)
	{
		lua_getglobal(luaState, "game");
		if (lua_isnil(luaState, -1)) { lua_pop(luaState, 1); return; };

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
		{
			lua_pop(luaState, 2);
		}
	}
	else if (evt == WSEEvent::GameLoad)
	{
		lua_getglobal(luaState, "game");
		if (lua_isnil(luaState, -1)) { lua_pop(luaState, 1); return; };

		lua_pushstring(luaState, "OnGameLoad");
		lua_rawget(luaState, -2);

		if (lua_type(luaState, -1) == LUA_TFUNCTION)
		{
			gameLoad_active = true;

			if (lua_pcall(luaState, 0, 0, 0))
			{
				printLastLuaError(luaState);
			}

			gameLoad_active = false;

			lua_pop(luaState, 1);
		}
		else
		{
			lua_pop(luaState, 2);
		}
	}
}

void WSELuaOperationsContext::hookOperation(lua_State *L, int opcode, int lRef)
{
	if (operationHookLuaRefs[opcode] != LUA_NOREF)
	{
		luaL_unref(L, LUA_REGISTRYINDEX, operationHookLuaRefs[opcode]);
		operationHookLuaRefs[opcode] = LUA_NOREF;

		if (opcode >= WSE_FIRST_WARBAND_OPCODE && opcode <= WSE_LAST_WARBAND_OPCODE)
			WSE->Hooks.UnhookJumptable(this, wb::addresses::operation_Execute_jumptable, opcode - 30);
	}

	if (lRef == LUA_NOREF) return;

	operationHookLuaRefs[opcode] = lRef;

	if (opcode >= WSE_FIRST_WARBAND_OPCODE && opcode <= WSE_LAST_WARBAND_OPCODE)
		WSE->Hooks.HookJumptable(this, wb::addresses::operation_Execute_jumptable, opcode - 30, LuaOperationJumptableHook);
}

void WSELuaOperationsContext::hookScript(lua_State *L, int script_no, int lRef)
{
	std::stringstream ss;
	ss << "Script [" << script_no << "] ";

	rgl::string id = ss.str().c_str();
	id += warband->script_manager.scripts[script_no].id;

	//WSE->Log.Info("hook %d, %d, %s", script_no, lRef, id.c_str());

	if (this->operationMgrHookLuaRefs.find(id) != this->operationMgrHookLuaRefs.end())
	{
		luaL_unref(L, LUA_REGISTRYINDEX, this->operationMgrHookLuaRefs[id]);
		this->operationMgrHookLuaRefs.erase(id);
	}

	if (lRef == LUA_NOREF) return;

	this->operationMgrHookLuaRefs[id] = lRef;
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
		if (lua_type(luaState, 2 + oldTop) == LUA_TBOOLEAN) //cf
			*continue_loop = lua_toboolean(luaState, 2 + oldTop) != 0;
		else if (lua_type(luaState, 2 + oldTop) == LUA_TNUMBER) //lhs
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
	else if (nResults == 3) //cf + lhs
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

bool WSELuaOperationsContext::OnOperationMgrExecute(wb::operation_manager *operation_manager, int& num_parameters, __int64* parameters)
{
	//WSE->Log.Info("check %s", operation_manager->id.c_str());
	auto hook = this->operationMgrHookLuaRefs.find(operation_manager->id);
	if (hook == this->operationMgrHookLuaRefs.end()) return true;
	//WSE->Log.Info("found %s", operation_manager->id.c_str());

	int ref = hook->second;
	if (ref == LUA_NOREF) return true;

	int oldTop = lua_gettop(luaState);

	lua_rawgeti(luaState, LUA_REGISTRYINDEX, ref);

	for (int i = 0; i < num_parameters; i++)
		lua_pushinteger(luaState, (lua_Integer)parameters[i]);

	if (lua_pcall(luaState, num_parameters, LUA_MULTRET, 0))
	{
		printLastLuaError(luaState);
		return true;
	}

	bool cont = true;
	int nResults = lua_gettop(luaState) - oldTop;
	if (nResults > 0)
	{
		if (lua_type(luaState, 1 + oldTop) == LUA_TBOOLEAN)
			cont = lua_toboolean(luaState, 1 + oldTop) != 0;
		else
		{
			if (nResults > MAX_NUM_STATEMENT_BLOCK_PARAMS)
				gPrint("too many return values");
			else
			{
				if (num_parameters > nResults)
					memset(parameters + nResults, 0, (MAX_NUM_STATEMENT_BLOCK_PARAMS - nResults) * sizeof(__int64));

				num_parameters = nResults;
				for (int i = 0; i < nResults; i++)
				{
					if (lua_type(luaState, 1 + oldTop) != LUA_TNUMBER)
					{
						gPrintf("invalid return value #%i, must be integer", i + 1);
						break;
					}
					parameters[i] = lua_tointeger(luaState, i + 1 + oldTop);
				}
			}
		}
	}

	lua_settop(luaState, oldTop);
	return cont;
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
				op->second->flags |= flag;
		}
	}
}

void WSELuaOperationsContext::loadOperations()
{
	std::string opFile = user_dir + "msfiles\\" + "header_operations.py";
	if (!fileExists(opFile))
		return;

	std::ifstream opStream(opFile);
	std::string curLine;
	int curLineNum = 0;

	std::smatch curMatches;

	std::regex opRegEx(R"((\w+)=(((0x)[\da-fA-F]+)|(\d+)).*)");
	std::regex opRefRegEx(R"((\w+)=(\w+).*)");
	std::regex opOrRegEx(R"((\w+)=(\w+)\|(\w+).*)");
	std::regex listStartRegEx(R"((\w+)(\+)?=\[.*)");

	std::unordered_map<std::string, std::vector<std::string>*> flagLists;

	while (std::getline(opStream, curLine))
	{
		curLineNum++;
		delBlank(curLine);

		if (curLine.length() == 0 || curLine[0] == '#')
			continue;

		try
		{
			if (std::regex_match(curLine, curMatches, opRegEx))
			{

				gameOperation* newOp = new gameOperation();
				newOp->flags = 0;

				if (curMatches.str(4).length())
					newOp->opcode = std::strtoul(curMatches.str(2).c_str(), 0, 16);
				else
					newOp->opcode = std::strtoul(curMatches.str(2).c_str(), 0, 10);

				operationMap[curMatches.str(1)] = newOp;
			}
			else if (std::regex_match(curLine, curMatches, opRefRegEx))
			{
				auto ref = operationMap.find(curMatches.str(2));

				if (ref == operationMap.end())
					gPrintf("WSELuaOperationsContext: Error reading %s, line %i: invalid reference", opFile.c_str(), curLineNum);
				else
					operationMap[curMatches.str(1)] = ref->second;
			}
			else if (std::regex_match(curLine, curMatches, listStartRegEx))
			{
				std::string listName = curMatches.str(1);
				bool add = curMatches.str(2) == "+";

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

				std::string curKeysStr = curLine;
				curKeysStr.erase(0, curKeysStr.find('[') + 1);

				size_t endPos = curKeysStr.find(']');
				if (endPos != std::string::npos) curKeysStr.erase(endPos);

				while (endPos == std::string::npos && std::getline(opStream, curLine))
				{
					curLineNum++;
					delBlank(curLine);
					discardComment(curLine);

					endPos = curLine.find(']');
					if (endPos != std::string::npos) curLine.erase(endPos);

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
					gameOperation* newOp = new gameOperation();
					newOp->opcode = op1->second->opcode | op2->second->opcode;
					newOp->flags = 0;

					operationMap[curMatches.str(1)] = newOp;
				}
			}
			else
			{
				gPrintf("WSELuaOperationsContext: Warning reading %s, could not process line %i", opFile.c_str(), curLineNum);
			}
		}
		catch (const std::regex_error& e)
		{
			gPrintf("WSELuaOperationsContext: Warning reading %s, exception while processing line %i: %s", opFile.c_str(), curLineNum, e.what());
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
		else //if (s != "header_operations.py")
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

void WSELuaOperationsContext::loadGlobalVars()
{
	std::string path = warband->cur_module_path;
	path += "\\variables.txt";
	if (!fileExists(path)) return;

	std::ifstream fStream(path);
	std::string curLine = "";
	int i = -1;

	while (std::getline(fStream, curLine))
	{
		i++;
		if (!curLine.length()) continue;
		gvarMap[curLine] = i;
	}
}

void WSELuaOperationsContext::initLua()
{
	//Find out directories for IO sandbox
	user_dir = warband->cur_module_path;
	std::replace(user_dir.begin(), user_dir.end(), '/', '\\'); //only backslash

	user_dir += (user_dir.back() == '\\') ? "lua\\" : "\\lua\\";

	storage_dir = this->CreateStorageDir();

	//Lets go
	luaState = luaL_newstate();
	lua_set_sandboxed_path_callback(luaState, sandbox_path);
	luaL_openlibs(luaState);

	luaopen_lanes_embedded(luaState, initLaneState, loadLanesLua);
	lua_pop(luaState, 1);

	loadOperations();
	loadGameConstants(user_dir + "msfiles\\");
	loadGlobalVars();

	initLGameTable(luaState);
	doMainScript();

	luaStateIsReady = true;
}

void WSELuaOperationsContext::doMainScript()
{
	std::string mainFile = user_dir + "main.lua";

	if (fileExists(mainFile))
	{
		lua_pushcfunction(luaState, traceback);

		if (luaL_loadfile(luaState, mainFile.c_str()))
		{
			printLastLuaError(luaState);
		}
		else
		{
			if (lua_pcall(luaState, 0, 0, -2))
				printLastLuaError(luaState);
		}

		lua_pop(luaState, 1);
	}
}
