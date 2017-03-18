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
		luaL_error(L, "undefined operation: [%s]", opName.c_str());

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
		else if (curLArgType == LUA_TTABLE && lIsPos(L, curLArgIndex))
		{
			warband->basic_game.position_registers[--curPosReg] = lToPos(L, curLArgIndex);
			wop.operands[curOperandIndex] = curPosReg;
		}
		else
			luaL_error(L, "invalid operand #%d", curOperandIndex);

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
		luaL_error(WSE->LuaOperations.luaState, "invalid template id: %s", tId);
	else
		warband->mission_templates[tNo].addTrigger(newT, tNo);

	return 0;
}

/***********
**Iterators
***********/
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

	int index = WSE->LuaOperations.gIteratorAdd(it);
	if (index == -1)
		luaL_error(L, "exceeded max game iterator limit (%d)", maxGiterators);

	lua_pushinteger(L, index);
	return 1;
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

	if (checkLArgs(L, 0, 2, lNum, lNum))
	{
		it.usePos = true;
		/*if (warband->cur_mission->grid.initialize_iterator(it.grid_iterator, POS, radius))
		{
		it.curVal = it.grid_iterator.agent_obj->agent->no;
		it.gridItSucc = true;
		}
		else
		it.gridItSucc = false;*/
	}
	else
	{
		it.usePos = false;
		it.curVal = warband->cur_mission->agents.get_first_valid_index();
	}

	int index = WSE->LuaOperations.gIteratorAdd(it);
	if (index == -1)
		luaL_error(L, "exceeded max game iterator limit (%d)", maxGiterators);

	lua_pushinteger(L, index);
	return 1;
}

void lPropInstIterAdvance(gameIterator *it)
{
	it->curVal = warband->cur_mission->mission_objects.get_next_valid_index(it->curVal);
	for (; it->curVal < warband->cur_mission->mission_objects.size(); it->curVal = warband->cur_mission->mission_objects.get_next_valid_index(it->curVal))
	{
		wb::mission_object *mission_object = &warband->cur_mission->mission_objects[it->curVal];

		if ((mission_object->meta_type == wb::mt_scene_prop || mission_object->meta_type == wb::mt_spawned_prop) && (it->subKindNo < 0 || mission_object->sub_kind_no == it->subKindNo))
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

	if (checkLArgs(L, 0, 1, lNum))
		it.subKindNo = lua_tointeger(L, 1);
	else
		it.subKindNo = -1;

	it.curVal = warband->cur_mission->mission_objects.get_first_valid_index();
	for (; it.curVal < warband->cur_mission->mission_objects.size(); it.curVal = warband->cur_mission->mission_objects.get_next_valid_index(it.curVal))
	{
		wb::mission_object *mission_object = &warband->cur_mission->mission_objects[it.curVal];

		if ((mission_object->meta_type == wb::mt_scene_prop || mission_object->meta_type == wb::mt_spawned_prop) && (it.subKindNo < 0 || mission_object->sub_kind_no == it.subKindNo))
			break;
	}

	int index = WSE->LuaOperations.gIteratorAdd(it);
	if (index == -1)
		luaL_error(L, "exceeded max game iterator limit (%d)", maxGiterators);

	lua_pushinteger(L, index);
	return 1;
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

	int index = WSE->LuaOperations.gIteratorAdd(it);
	if (index == -1)
		luaL_error(L, "exceeded max game iterator limit (%d)", maxGiterators);

	lua_pushinteger(L, index);
	return 1;
}

int lIterNext(lua_State *L)
{
	checkLArgs(L, 1, 1, lNum);
	int itNo = lua_tointeger(L, 1);

	gameIterator *it = WSE->LuaOperations.getGiterator(itNo);

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

/**************
**Iterators end
***************/