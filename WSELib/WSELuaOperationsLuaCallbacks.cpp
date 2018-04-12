#include <chrono>

#include "WSELuaOperationsLuaCallbacks.h"
#include "WSELuaOperationsHelpers.h"

int lGameExecOperationHandler(lua_State *L)
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
		luaL_error(L, "undefined module system operation: [%s]", opName.c_str());

	gameOperation op = opEntry->second;

	wb::operation wop;
	wop.opcode = op.opcode;
	wop.num_operands = numLArgs - 1;
	wop.end_statement = -1;

	__int64 locals[1];

	int curStrReg = NUM_REGISTERS;
	int curPosReg = NUM_REGISTERS;

	int curLArgIndex = 2;
	int curOperandIndex = 0;

	if (op.flags & WSEOperationType::LhsOperation)
	{
		locals[0] = lua_tointeger(L, curLArgIndex++);
		setOperandToLocalVar(wop.operands[curOperandIndex++], 0);
	}

	while (curLArgIndex <= numLArgs)
	{
		int curLArgType = lua_type(L, curLArgIndex);

		if (curLArgType == LUA_TNUMBER)
			wop.operands[curOperandIndex] = lua_tointeger(L, curLArgIndex);
		else if (curLArgType == LUA_TSTRING)
		{
			warband->basic_game.string_registers[--curStrReg] = lua_tostring(L, curLArgIndex);
			wop.operands[curOperandIndex] = curStrReg;
		}
		else if (lIsPos(L, curLArgIndex))
		{
			warband->basic_game.position_registers[--curPosReg] = lToPos(L, curLArgIndex);
			wop.operands[curOperandIndex] = curPosReg;
		}
		else
			luaL_error(L, "invalid operand #%d", curOperandIndex);

		curOperandIndex++;
		curLArgIndex++;
	}

	if (op.opcode == wb::opcodes::call_script)
	{
		if (wop.operands[0] >= 0 && wop.operands[0] < warband->script_manager.num_scripts)
		{
			lua_pushboolean(L, warband->script_manager.scripts[wop.operands[0]].execute(wop.num_operands - 1, &wop.operands[1]));
			return 1;
		}
		else
		{
			char buf[1000];
			sprintf_s(buf, "invalid script no %lld", wop.operands[0]);
			luaL_error(L, buf);
		}
	}
	else
	{
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
	}

	return 0;
}

int lGetRegHandler(lua_State *L)
{
	checkLArgs(L, 2, 2, lNum, lNum);

	int typeId = lua_tointeger(L, 1);
	int index = lua_tointeger(L, 2);

	if (index < 0 || index >= NUM_REGISTERS)
		luaL_error(L, "index out of range");

	if (typeId == 0)
		lua_pushinteger(L, (lua_Integer)warband->basic_game.registers[index]);
	else if (typeId == 1)
		lua_pushstring(L, warband->basic_game.string_registers[index]);
	else if (typeId == 2)
		lPushPos(L, warband->basic_game.position_registers[index]);

	return 1;
}

int lSetRegHandler(lua_State *L)
{
	if (lua_gettop(L) != 3) //TODO -- why max?
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
	else if (typeId == 2)
	{
		if (lIsPos(L, 3))
			warband->basic_game.position_registers[index] = lToPos(L, 3);
		else
			luaL_error(L, "val is not pos");
	}

	return 0;
}

int lGetScriptNo(lua_State *L)
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

int lGetCurTemplateId(lua_State *L)
{
	lua_pushstring(L, warband->mission_templates[warband->cur_mission->cur_mission_template_no].id);
	return 1;
}

int lAddTrigger(lua_State *L)
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

	if (numArgs == 6)
	{
		newT.consequences.num_operations = 1;
		newT.consequences.operations = rgl::_new<wb::operation>(1);

		newT.consequences.operations[0].opcode = WSE->LuaOperations.callTriggerOpcode;
		newT.consequences.operations[0].num_operands = 1;

		newT.consequences.operations[0].operands[0] = luaL_ref(L, LUA_REGISTRYINDEX);
	}

	newT.conditions.num_operations = 1;
	newT.conditions.operations = rgl::_new<wb::operation>(1);

	newT.conditions.operations[0].opcode = WSE->LuaOperations.callTriggerOpcode;
	newT.conditions.operations[0].num_operands = 1;

	newT.conditions.operations[0].operands[0] = luaL_ref(L, LUA_REGISTRYINDEX);

	int tNo = getTemplateNo(tId);

	if (tNo < 0)
		luaL_error(L, "invalid template id: %s", tId);
	
	int index = warband->mission_templates[tNo].addTrigger(newT, tNo);

	lua_pushinteger(L, index);
	return 1;
}

int lRemoveTrigger(lua_State *L)
{
	int numArgs = checkLArgs(L, 2, 2, lStr, lNum);

	const char *tId = lua_tostring(L, 1);
	int index = lua_tointeger(L, 2);

	int tNo = getTemplateNo(tId);
	if (tNo < 0)
		luaL_error(WSE->LuaOperations.luaState, "invalid template id: %s", tId);

	bool succ = warband->mission_templates[tNo].removeTrigger(index);

	lua_pushboolean(L, succ ? 1 : 0);
	return 1;
}

int lAddItemTrigger(lua_State *L)
{
	int numArgs = checkLArgs(L, 3, 3, lStr|lNum, lNum, lFunc);

	int itmNo;
	if (lua_type(L, 1) == LUA_TSTRING)
	{
		const char *itmID = lua_tostring(L, 1);
		itmNo = getItemKindNo(itmID);

		if (itmNo < 0)
			luaL_error(L, "invalid item kind id: %s", itmID);
	}
	else
	{
		itmNo = lua_tointeger(L, 1);
		if (itmNo < 0 || itmNo >= warband->num_item_kinds)
			luaL_error(L, "invalid item kind no: %d", itmNo);
	}

	wb::simple_trigger newT;

	newT.interval = (float)lua_tonumber(L, 2);
	newT.interval_timer = rgl::timer();

	newT.operations.num_operations = 1;
	newT.operations.operations = rgl::_new<wb::operation>(1);

	newT.operations.operations[0].opcode = WSE->LuaOperations.callTriggerOpcode;
	newT.operations.operations[0].num_operands = 1;

	newT.operations.operations[0].operands[0] = luaL_ref(L, LUA_REGISTRYINDEX);

	int index = warband->item_kinds[itmNo].simple_triggers.addTrigger(newT);

	lua_pushinteger(L, index);
	return 1;
}

#if defined WARBAND
int lAddPrsnt(lua_State *L)
{
	checkTableStructure(L, 1, "{id=str, [flags]={(val=num)}, [mesh]=num, triggers={(key=num, val=func, min=1)} }");

	wb::presentation newP = *rgl::_new<wb::presentation>();

	lua_getfield(L, 1, "id");
	newP.id.initialize();
	newP.id = lua_tostring(L, -1);
	lua_pop(L, 1);

	newP.mesh_no = 0;
	lua_getfield(L, 1, "mesh");
	if (lua_type(L, -1))
		newP.mesh_no = lua_tointeger(L, -1);
	lua_pop(L, 1);

	newP.flags = 0;
	lua_getfield(L, 1, "flags");
	if (lua_type(L, -1))
	{
		lua_pushnil(L);
		while (lua_next(L, -2))
		{
			newP.flags |= lua_tointeger(L, -1);
			lua_pop(L, 1);
		}
	}
	lua_pop(L, 1);

	lua_getfield(L, 1, "triggers");

	int numTriggers = 0;
	lua_pushnil(L);
	while (lua_next(L, -2))
	{
		numTriggers++;
		lua_pop(L, 1);
	}

	newP.simple_triggers.num_simple_triggers = numTriggers;
	newP.simple_triggers.simple_triggers = (wb::simple_trigger*)malloc(sizeof(wb::simple_trigger) * numTriggers);

	int i = 0;
	lua_pushnil(L);
	while (lua_next(L, -2))
	{
		wb::simple_trigger &curTrigger = newP.simple_triggers.simple_triggers[i];

		curTrigger.interval = (float)lua_tonumber(L, -2);
		curTrigger.interval_timer = rgl::timer(2);
		
		curTrigger.operations.id.initialize();
		curTrigger.operations.num_operations = 1;
		curTrigger.operations.operations = rgl::_new<wb::operation>(1);
		curTrigger.operations.operations[0].opcode = WSE->LuaOperations.callTriggerOpcode;
		curTrigger.operations.operations[0].num_operands = 1;	
		curTrigger.operations.operations[0].operands[0] = luaL_ref(L, LUA_REGISTRYINDEX); //pops val

		i++;
	}
	lua_pop(L, 1);

	int index = warband->presentation_manager.addPresentation(newP);
	lua_pushinteger(L, index);
	return 1;
}

int lRemovePrsnt(lua_State *L)
{
	int numArgs = checkLArgs(L, 1, 1, lNum);

	bool succ = warband->presentation_manager.removePresentation(lua_tointeger(L, 1));

	lua_pushboolean(L, succ ? 1 : 0);
	return 1;
}
#endif

/***********
**Iterators
***********/
struct gameIterator
{
	bool valid;

	void(*advance)(gameIterator *it);
	bool(*curValIsValid)(gameIterator *it);

	int curVal;

	bool usePos;
	bool gridItSucc;
	wb::mission_grid_iterator grid_iterator;

	int subKindNo;
	int metaType;
};

int lIterNext(lua_State *L)
{
	gameIterator *it = (gameIterator*)lua_touserdata(L, lua_upvalueindex(1));

	if (it && it->curValIsValid(it))
	{
		lua_pushnumber(L, it->curVal);

		it->advance(it);
	}
	else
	{
		if (it)
			it->valid = false;

		lua_pushnil(L);
	}

	return 1;
}

int lPushIterator(lua_State *L, const gameIterator &it)
{
	gameIterator *ud = (gameIterator*)lua_newuserdata(L, sizeof(gameIterator));
	*ud = it;

	lua_pushcclosure(L, lIterNext, 1);
	return 1;
}

void lPartiesIterAdvance(gameIterator *it)
{
	it->curVal = warband->cur_game->parties.get_next_valid_index(it->curVal);
}

bool lPartiesIterCurValIsValid(gameIterator *it)
{
	return it->curVal < warband->cur_game->parties.num_created;
}

int lPartiesIterInit(lua_State *L)
{
	gameIterator it;
	it.valid = true;
	it.advance = lPartiesIterAdvance;
	it.curValIsValid = lPartiesIterCurValIsValid;

	it.curVal = warband->cur_game->parties.get_first_valid_index();

	return lPushIterator(L, it);
}

void lAgentsIterAdvance(gameIterator *it)
{
	if (it->usePos)
	{
		if (warband->cur_mission->grid.advance_iterator(it->grid_iterator))
		{
			it->curVal = it->grid_iterator.agent_obj->agent->no;
			it->gridItSucc = true;
		}
		else
			it->gridItSucc = false;
	}
	else
	{
		it->curVal = warband->cur_mission->agents.get_next_valid_index(it->curVal);
	}
}

bool lAgentsIterCurValIsValid(gameIterator *it)
{
	if (it->usePos)
		return it->gridItSucc;
	else
		return it->curVal < warband->cur_mission->agents.size();
}

int lAgentsIterInit(lua_State *L)
{
	gameIterator it;
	it.valid = true;
	it.advance = lAgentsIterAdvance;
	it.curValIsValid = lAgentsIterCurValIsValid;

	if (checkLArgs(L, 0, 2, lNum|lPos, lNum))
	{
		if (lua_gettop(L) < 2)
			luaL_error(L, "not enough arguments");

		it.usePos = true;
		rgl::matrix pos;

		if (lua_isnumber(L, 1))
		{
			int preg = lua_tointeger(L, 1);
			if (preg < 0 || preg >= NUM_REGISTERS)
				luaL_error(L, "index out of range");
			pos = warband->basic_game.position_registers[preg];
		}
		else
			pos = lToPos(L, 1);

		if (warband->cur_mission->grid.initialize_iterator(it.grid_iterator, pos.o, (float)lua_tonumber(L, 2)))
		{
			it.curVal = it.grid_iterator.agent_obj->agent->no;
			it.gridItSucc = true;
		}
		else
			it.gridItSucc = false;
	}
	else
	{
		it.usePos = false;
		it.curVal = warband->cur_mission->agents.get_first_valid_index();
	}

	return lPushIterator(L, it);
}

void lPropInstIterAdvance(gameIterator *it)
{
	it->curVal = warband->cur_mission->mission_objects.get_next_valid_index(it->curVal);
	for (; it->curVal < warband->cur_mission->mission_objects.size(); it->curVal = warband->cur_mission->mission_objects.get_next_valid_index(it->curVal))
	{
		wb::mission_object *mission_object = &warband->cur_mission->mission_objects[it->curVal];

		if ((it->subKindNo <= 0 || mission_object->sub_kind_no == it->subKindNo) && (it->metaType <= 0 || mission_object->meta_type == it->metaType - 1))
			break;
	}
}

bool lPropInstIterCurValIsValid(gameIterator *it)
{
	return it->curVal < warband->cur_mission->mission_objects.size();
}

int lPropInstIterInit(lua_State *L)
{
	gameIterator it;
	it.valid = true;
	it.advance = lPropInstIterAdvance;
	it.curValIsValid = lPropInstIterCurValIsValid;

	it.subKindNo = 0;
	it.metaType = 0;

	if (checkLArgs(L, 0, 2, lNum))
	{
		it.subKindNo = lua_tointeger(L, 1);
		it.metaType = lua_tointeger(L, 2);
	}

	it.curVal = warband->cur_mission->mission_objects.get_first_valid_index();
	for (; it.curVal < warband->cur_mission->mission_objects.size(); it.curVal = warband->cur_mission->mission_objects.get_next_valid_index(it.curVal))
	{
		wb::mission_object *mission_object = &warband->cur_mission->mission_objects[it.curVal];

		if ((it.subKindNo <= 0 || mission_object->sub_kind_no == it.subKindNo) && (it.metaType <= 0 || mission_object->meta_type == it.metaType - 1))
			break;
	}

	return lPushIterator(L, it);
}

void lPlayersIterAdvance(gameIterator *it)
{
	for (it->curVal++; it->curVal < NUM_NETWORK_PLAYERS; it->curVal++)
	{
		wb::network_player *player = &warband->multiplayer_data.players[it->curVal];

		if (player->is_active() && player->ready)
			break;
	}
}

bool lPlayersIterCurValIsValid(gameIterator *it)
{
	return it->curVal < NUM_NETWORK_PLAYERS;
}

int lPlayersIterInit(lua_State *L)
{
	gameIterator it;
	it.valid = true;
	it.advance = lPlayersIterAdvance;
	it.curValIsValid = lPlayersIterCurValIsValid;

	it.curVal = (checkLArgs(L, 0, 1, lNum) && lua_tointeger(L, 1)) ? 1 : 0;

	for (; it.curVal < NUM_NETWORK_PLAYERS; it.curVal++)
	{
		wb::network_player *player = &warband->multiplayer_data.players[it.curVal];

		if (player->is_active() && player->ready)
			break;
	}

	return lPushIterator(L, it);
}

/**************
**Iterators end
***************/

int lPrint(lua_State *L)
{
	checkLArgs(L, 1, 1, lStr);

	/*gPrintf("pointer to multiplayer_data.players: %p", (void*)&warband->multiplayer_data.players);

	for(int i = 0; i < warband->multiplayer_data.num_players; i++)
	{
		wb::network_player *curPlayer = &warband->multiplayer_data.players[i];
		//if (curPlayer->status == wb::nps_active && curPlayer->ready)
		//{
			gPrintf("pointer to player %d events vector: %p", i, (void*)&curPlayer->events);
		//}
	}*/

	#if defined WARBAND
		warband->window_manager.display_message(lua_tostring(L, 1), 0xFFFF5555, 0);
	#else
		const char *str = lua_tostring(L, 1);
		DWORD a = 0;
		WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), str, strlen(str), &a, NULL);
	#endif

	return 0;
}

int lHookOperation(lua_State *L)
{
	checkLArgs(L, 2, 2, lNum|lStr, lFunc);

	int opcode;
	if (lua_type(L, 1) == LUA_TNUMBER)
	{
		opcode = lua_tointeger(L, 1);

		if (opcode < 0 || opcode >= WSE_MAX_NUM_OPERATIONS)
			luaL_error(L, "opcode %d out of range", opcode);
	}
	else
	{
		std::string opName = lua_tostring(L, 1);

		auto opEntry = WSE->LuaOperations.operationMap.find(opName);

		if (opEntry == WSE->LuaOperations.operationMap.end())
			luaL_error(L, "undefined module system operation: [%s]", opName.c_str());

		opcode = opEntry->second.opcode;
	}

	WSE->LuaOperations.hookOperation(L, opcode, luaL_ref(L, LUA_REGISTRYINDEX));

	return 0;
}

int lUnhookOperation(lua_State *L)
{
	checkLArgs(L, 2, 2, lNum | lStr);

	int opcode;
	if (lua_type(L, 1) == LUA_TNUMBER)
	{
		opcode = lua_tointeger(L, 1);

		if (opcode < 0 || opcode >= WSE_MAX_NUM_OPERATIONS)
			luaL_error(L, "opcode %d out of range", opcode);
	}
	else
	{
		std::string opName = lua_tostring(L, 1);

		auto opEntry = WSE->LuaOperations.operationMap.find(opName);

		if (opEntry == WSE->LuaOperations.operationMap.end())
			luaL_error(L, "undefined module system operation: [%s]", opName.c_str());

		opcode = opEntry->second.opcode;
	}
	if (!WSE->LuaOperations.unhookOperation(L, opcode))
		luaL_error(L, "Unable to unhook opcode [%d].", opcode);

	return 0;
}

int lGetTime(lua_State *L)
{
	using namespace std::chrono;
	uint64_t t = duration_cast<milliseconds>(steady_clock::now() - WSE->LuaOperations.tStart).count();
	lua_pushinteger(L, (LUA_INTEGER)t);

	return 1;
}

int lFailMsCall(lua_State *L)
{
	WSE->LuaOperations.dontFailMsCall = false;
	return 0;
}