#include <chrono>
#include <math.h>
#include <sstream>
#include <string>
#define _USE_MATH_DEFINES

#include "WSELua_Callbacks.h"
#include "WSELua_Helpers.h"
#include "WSELua_Iterators.h"
#include "lanes.h"
#include "luaSockets/src/luasocket.h"
#include "LSQLite3/lsqlite3.h"
#include "WSELib.rc.h"
#include "lua-std-regex/lregex.h"

typedef std::pair<const char*, lua_CFunction> callback_def; //name, callback()

static std::vector<callback_def> _G_game_callbacks;
static std::vector<callback_def> _G_callbacks;

REG(execOperation)
int lc_execOperation(lua_State *L)
{
	int numLArgs = lua_gettop(L);

	if (numLArgs == 0)
		luaL_error(L, "need operation identifier");
	else if (numLArgs > MAX_NUM_STATEMENT_OPERANDS + 1)
		luaL_error(L, "operand count can't be > %d", MAX_NUM_STATEMENT_OPERANDS);

	if (!lua_isstring(L, 1))
		luaL_error(L, "invalid operation identifier");

	std::string opName(lua_tostring(L, 1));

	auto opEntry = WSE->LuaOperations.operationMap.find(opName);

	if (opEntry == WSE->LuaOperations.operationMap.end())
		luaL_error(L, "undefined module system operation: '%s'", opName.c_str());

	gameOperation op = *(opEntry->second);

	wb::operation wop;
	wop.opcode = op.opcode;
	wop.num_operands = numLArgs - 1;
	wop.end_statement = -1;

	__int64 locals[1];

	int curStrReg = NUM_REGISTERS;
	int curPosReg = NUM_REGISTERS;

	int curLArgIndex = 2;
	int curOperandIndex = 0;

	if (op.flags & WSEOperationFlags::Lhs)
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
			luaL_error(L, "invalid operand #%d to module operation '%s'", curOperandIndex + 1, opName.c_str());

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

		bool b = wop.execute(locals, WSE->LuaOperations.luaContext, e);

		int retCount = 1;
		if (op.flags & WSEOperationFlags::Cf)
		{
			lua_pushboolean(L, b);
			retCount++;
		}

		if (op.flags & WSEOperationFlags::Lhs)
		{
			lua_pushinteger(L, (lua_Integer)locals[0]);
			retCount++;
		}

		lua_pushinteger(L, e);

		return retCount;
	}

	return 0;
}

REG(getOperationFlags)
int lc_getOperationFlags(lua_State *L)
{
	checkLArgs(L, 1, 1, lStr);

	std::string opName(lua_tostring(L, 1));

	auto opEntry = WSE->LuaOperations.operationMap.find(opName);
	if (opEntry == WSE->LuaOperations.operationMap.end())
		luaL_error(L, "undefined module system operation: '%s'", opName.c_str());

	gameOperation op = *(opEntry->second);
	lua_pushinteger(L, op.flags);

	return 1;
}

REG(getReg)
int lc_getReg(lua_State *L)
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

REG(setReg)
int lc_setReg(lua_State *L)
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

REG(getGvar)
int lc_getGvar(lua_State *L)
{
	checkLArgs(L, 1, 1, lStr);

	std::string gvar = lua_tostring(L, 1);

	if (WSE->LuaOperations.gvarMap.find(gvar) == WSE->LuaOperations.gvarMap.end())
		luaL_error(L, "invalid gvar '%s'", gvar.c_str());

	int idx = WSE->LuaOperations.gvarMap[gvar];
	lua_pushinteger(L, (lua_Integer)warband->basic_game.global_variables.get(idx));

	return 1;
}

REG(setGvar)
int lc_setGvar(lua_State *L)
{
	checkLArgs(L, 2, 2, lStr, lNum);

	std::string gvar = lua_tostring(L, 1);
	lua_Integer val = lua_tointeger(L, 2);

	if (WSE->LuaOperations.gvarMap.find(gvar) == WSE->LuaOperations.gvarMap.end())
		luaL_error(L, "invalid gvar '%s'", gvar.c_str());

	int idx = WSE->LuaOperations.gvarMap[gvar];
	warband->basic_game.global_variables.set(idx, val);

	return 0;
}

REG(getScriptNo)
int lc_getScriptNo(lua_State *L)
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

REG(getCurTemplateNo)
int lc_getCurTemplateNo(lua_State *L)
{
	lua_pushinteger(L, warband->cur_mission->cur_mission_template_no);
	return 1;
}

REG(getCurTemplateId)
int lc_getCurTemplateId(lua_State *L)
{
	lua_pushstring(L, warband->mission_templates[warband->cur_mission->cur_mission_template_no].id);
	return 1;
}

REG(getNumTemplates)
int lc_getNumTemplates(lua_State *L)
{
	lua_pushinteger(L, warband->num_mission_templates);
	return 1;
}

REG(getTemplateId)
int lc_getTemplateId(lua_State *L)
{
	checkLArgs(L, 1, 1, lNum);

	int tNo = lua_tointeger(L, 1);
	if (tNo < 0 || tNo >= warband->num_mission_templates)
		luaL_error(L, "invalid template no: %d", tNo);

	lua_pushstring(L, warband->mission_templates[tNo].id);
	return 1;
}

/* Triggers */
REG(addTrigger)
int lc_addTrigger(lua_State *L)
{
	int numArgs = checkLArgs(L, 5, 6, lStr | lNum, lNum, lNum, lNum, lFunc, lFunc);

	int tNo = lToTemplateNo(L, 1);

	wb::trigger trigg;
	lFillTrigger(L, trigg, numArgs == 6, 2, rgl::timer_kind::mission);

	int index = warband->mission_templates[tNo].add_trigger(trigg, tNo, " (Lua)");

	lua_pushinteger(L, index);
	return 1;
}

REG(removeTrigger)
int lc_removeTrigger(lua_State *L)
{
	int numArgs = checkLArgs(L, 2, 2, lStr | lNum, lNum);

	int tNo = lToTemplateNo(L, 1);
	int index = lua_tointeger(L, 2);

	bool succ = warband->mission_templates[tNo].remove_trigger(index);

	lua_pushboolean(L, succ ? 1 : 0);
	return 1;
}

REG(getNumTriggers)
int lc_getNumTriggers(lua_State *L)
{
	int numArgs = checkLArgs(L, 1, 1, lStr | lNum);
	int tNo = lToTemplateNo(L, 1);

	lua_pushinteger(L, warband->mission_templates[tNo].triggers.num_triggers);
	return 1;
}

REG(addItemTrigger)
int lc_addItemTrigger(lua_State *L)
{
	int numArgs = checkLArgs(L, 3, 3, lStr | lNum, lNum, lFunc);
	int itm_no = lToItemNo(L, 1);

	wb::simple_trigger trigg;
	lFillSimpleTrigger(L, trigg, 2);
	trigg.operations.id.format("Item Kind [%d] %s Trigger [%d] (Lua)", itm_no, warband->item_kinds[itm_no].id.c_str(), warband->item_kinds[itm_no].simple_triggers.num_simple_triggers);

	int index = warband->item_kinds[itm_no].simple_triggers.add_trigger(trigg);

	lua_pushinteger(L, index);
	return 1;
}

REG(removeItemTrigger)
int lc_removeItemTrigger(lua_State *L)
{
	int numArgs = checkLArgs(L, 2, 2, lStr | lNum, lNum);
	int itm_no = lToItemNo(L, 1);
	int index = lua_tointeger(L, 2);

	bool succ = warband->item_kinds[itm_no].simple_triggers.remove_trigger(index);

	lua_pushboolean(L, succ ? 1 : 0);
	return 1;
}

REG(getNumItemTriggers)
int lc_getNumItemTriggers(lua_State *L)
{
	int numArgs = checkLArgs(L, 1, 1, lStr | lNum);
	int itm_no = lToItemNo(L, 1);

	lua_pushinteger(L, warband->item_kinds[itm_no].simple_triggers.num_simple_triggers);
	return 1;
}

REG(addScenePropTrigger)
int lc_addScenePropTrigger(lua_State *L)
{
	int numArgs = checkLArgs(L, 3, 3, lStr | lNum, lNum, lFunc);
	int prop_no = lToScenePropNo(L, 1);

	wb::simple_trigger trigg;
	lFillSimpleTrigger(L, trigg, 2, rgl::timer_kind::mission);
	trigg.operations.id.format("Scene Prop [%d] %s Trigger [%d] (Lua)", prop_no, warband->scene_props[prop_no].id.c_str(), warband->scene_props[prop_no].simple_triggers.num_simple_triggers);

	int index = warband->scene_props[prop_no].simple_triggers.add_trigger(trigg);

	lua_pushinteger(L, index);
	return 1;
}

REG(removeScenePropTrigger)
int lc_removeScenePropTrigger(lua_State *L)
{
	int numArgs = checkLArgs(L, 2, 2, lStr | lNum, lNum);
	int prop_no = lToScenePropNo(L, 1);
	int index = lua_tointeger(L, 2);

	bool succ = warband->scene_props[prop_no].simple_triggers.remove_trigger(index);

	lua_pushboolean(L, succ ? 1 : 0);
	return 1;
}

REG(getNumScenePropTriggers)
int lc_getNumScenePropTriggers(lua_State *L)
{
	int numArgs = checkLArgs(L, 1, 1, lStr | lNum);
	int prop_no = lToScenePropNo(L, 1);

	lua_pushinteger(L, warband->scene_props[prop_no].simple_triggers.num_simple_triggers);
	return 1;
}

//REG(addSimpleWorldTrigger)
//int lc_addSimpleWorldTrigger(lua_State *L)
//{
//	int numArgs = checkLArgs(L, 2, 2, lNum, lFunc);
//	if (!WSE->LuaOperations.gameLoad_active){ luaL_error(L, "addSimpleWorldTrigger: only allowed during OnGameLoad"); }
//
//	wb::simple_trigger trigg;
//	lFillSimpleTrigger(L, trigg, 1, rgl::timer_kind::game);
//	trigg.operations.id.format("Simple Trigger [%d] (Lua)", warband->cur_game->simple_triggers.num_simple_triggers);
//
//	int index = warband->cur_game->simple_triggers.add_trigger(trigg);
//
//	lua_pushinteger(L, index);
//	return 1;
//}
//
//REG(removeSimpleWorldTrigger)
//int lc_removeSimpleWorldTrigger(lua_State *L)
//{
//	int numArgs = checkLArgs(L, 1, 1, lNum);
//	if (!WSE->LuaOperations.gameLoad_active){ luaL_error(L, "removeSimpleWorldTrigger: only allowed during OnGameLoad"); }
//
//	int index = lua_tointeger(L, 1);
//	bool succ = warband->cur_game->simple_triggers.remove_trigger(index);
//
//	lua_pushboolean(L, succ ? 1 : 0);
//	return 1;
//}
//
//REG(getNumSimpleWorldTriggers)
//int lc_getNumSimpleWorldTriggers(lua_State *L)
//{
//	lua_pushinteger(L, warband->cur_game->simple_triggers.num_simple_triggers);
//	return 1;
//}
//
//REG(addWorldTrigger)
//int lc_addWorldTrigger(lua_State *L)
//{
//	int numArgs = checkLArgs(L, 4, 5, lNum, lNum, lNum, lFunc, lFunc);
//	if (!WSE->LuaOperations.gameLoad_active){ luaL_error(L, "addWorldTrigger: only allowed during OnGameLoad"); }
//
//	wb::trigger trigg;
//	lFillTrigger(L, trigg, numArgs == 5, 1, rgl::timer_kind::game);
//	trigg.consequences.id.format("Trigger [%d] consequences (Lua)", warband->cur_game->triggers.num_triggers);
//	trigg.conditions.id.format("Trigger [%d] conditions (Lua)", warband->cur_game->triggers.num_triggers);
//
//	int index = warband->cur_game->triggers.add_trigger(trigg);
//
//	lua_pushinteger(L, index);
//	return 1;
//}
//
//REG(removeWorldTrigger)
//int lc_removeWorldTrigger(lua_State *L)
//{
//	int numArgs = checkLArgs(L, 1, 1, lNum);
//	if (!WSE->LuaOperations.gameLoad_active){ luaL_error(L, "removeWorldTrigger: only allowed during OnGameLoad"); }
//
//	int index = lua_tointeger(L, 1);
//	bool succ = warband->cur_game->triggers.remove_trigger(index);
//
//	lua_pushboolean(L, succ ? 1 : 0);
//	return 1;
//}
//
//REG(getNumWorldTriggers)
//int lc_getNumWorldTriggers(lua_State *L)
//{
//	lua_pushinteger(L, warband->cur_game->triggers.num_triggers);
//	return 1;
//}

//REG(dump_triggers)
int lc_dump_triggers(lua_State *L){
 	wb::trigger_manager* tigs;
 	wb::simple_trigger_manager* sigs;
 
 	int mst = warband->cur_mission->cur_mission_template_no;
 
 	if (mst >= 0 && mst < warband->num_mission_templates)
 	{
 		tigs = &(warband->mission_templates[mst].triggers);
 		gPrint("###mst###");
 		for (int i = 0; i < tigs->num_triggers; i++)
 		{
 			gPrintf("  %s, check_interval_timer.timer_no %d", tigs->triggers[i].conditions.id.c_str(), tigs->triggers[i].check_interval_timer.timer_no);
 		}
 	}
 
	gPrint("###### Items #####");
	for (int itm = 0; itm < warband->num_item_kinds; itm++)
	{
		sigs = &(warband->item_kinds[itm].simple_triggers);
		for (int i = 0; i < sigs->num_simple_triggers; i++)
		{
			gPrintf("  %s, check_interval_timer.timer_no %d", sigs->simple_triggers[i].operations.id.c_str(), sigs->simple_triggers[i].interval_timer.timer_no);
		}
	}
 	
 
	gPrint("####### Scene Props ########");
	for (int spr = 0; spr < warband->num_scene_props; spr++)
	{
		sigs = &(warband->scene_props[spr].simple_triggers);
		for (int i = 0; i < sigs->num_simple_triggers; i++)
		{
			gPrintf("  %s, check_interval_timer.timer_no %d", sigs->simple_triggers[i].operations.id.c_str(), sigs->simple_triggers[i].interval_timer.timer_no);
		}
	}
 
 
	if (warband->cur_game)
	{
		gPrint("###### world ######");
		for (int i = 0; i < warband->cur_game->triggers.num_triggers; i++)
		{
			gPrintf("  %s, check_interval_timer.timer_no %d", warband->cur_game->triggers.triggers[i].conditions.id.c_str(), warband->cur_game->triggers.triggers[i].check_interval_timer.timer_no);
		}

		gPrint("###### simple world ######");
		for (int i = 0; i < warband->cur_game->simple_triggers.num_simple_triggers; i++)
		{
			gPrintf("  %s, interval_timer.timer_no %d", warband->cur_game->simple_triggers.simple_triggers[i].operations.id.c_str(), warband->cur_game->simple_triggers.simple_triggers[i].interval_timer.timer_no);
		}
	}
 
 	return 0;
 }
/* End Triggers */

REG(addPrsnt)
int lc_addPrsnt(lua_State *L)
{
#if defined WARBAND
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
	newP.simple_triggers.simple_triggers = rgl::_new<wb::simple_trigger>(numTriggers);

	int i = 0;
	lua_pushnil(L);
	while (lua_next(L, -2))
	{
		wb::simple_trigger &curTrigger = newP.simple_triggers.simple_triggers[i];
		lFillSimpleTrigger(L, curTrigger, -2, rgl::timer_kind::mission);

		i++;
	}
	lua_pop(L, 1);

	int index = warband->presentation_manager.addPresentation(newP);
	lua_pushinteger(L, index);
	return 1;
#else
	return 0;
#endif
}

REG(removePrsnt)
int lc_removePrsnt(lua_State *L)
{
#if defined WARBAND
	int numArgs = checkLArgs(L, 1, 1, lNum);

	bool succ = warband->presentation_manager.removePresentation(lua_tointeger(L, 1));

	lua_pushboolean(L, succ ? 1 : 0);
	return 1;
#else
	return 0;
#endif
}

REG(addPsys)
int lc_addPsys(lua_State *L)
{
#if defined WARBAND
	//WSE->Log.Info("psys check format...");
	checkTableStructure(L, 1,
		"{id=str, [flags]={(val=num)}, mesh=str|num,\
			num_particles=num, life=num, damping=num, gravity_strength=num, turbulance_size=num, turbulance_strength=num,\
			alpha_keys			= {1={1=num, 2=num}, 2={1=num, 2=num}},\
			red_keys			= {1={1=num, 2=num}, 2={1=num, 2=num}},\
			green_keys			= {1={1=num, 2=num}, 2={1=num, 2=num}},\
			blue_keys			= {1={1=num, 2=num}, 2={1=num, 2=num}},\
			scale_keys			= {1={1=num, 2=num}, 2={1=num, 2=num}},\
			emit_box_size		= {1=num, 2=num, 3=num},\
			emit_velocity		= {1=num, 2=num, 3=num},\
			emit_dir_randomness = num,\
			rotation_speed		= num,\
			rotation_damping	= num\
		}");
	//WSE->Log.Info("format good");

	wb::particle_system new_sys = *rgl::_new<wb::particle_system>();

	lua_getfield(L, 1, "id");
	new_sys.id.initialize();
	new_sys.id = lua_tostring(L, -1);
	lua_pop(L, 1);


	new_sys.mesh_name.initialize();

	lua_getfield(L, 1, "mesh");
	if (lua_type(L, -1) == LUA_TSTRING)
	{
		new_sys.mesh_name = lua_tostring(L, -1);
		lua_pop(L, 1);

		new_sys.mesh = warband->resource_manager.try_get_mesh(new_sys.mesh_name, WSE->ModuleSettingsIni.Bool("", "use_case_insensitive_mesh_searches"));

		if (new_sys.mesh == nullptr){
			rgl::string name = new_sys.mesh_name;
			rgl::_free((void*)&new_sys);
			luaL_error(L, "addPsys: Could not find mesh %s", name.c_str());
		}
	}
	else
	{
		int idx = lua_tointeger(L, -1);
		lua_pop(L, 1);

		if (idx < 0 || idx >= warband->resource_manager.meshes.size())
		{
			rgl::_free((void*)&new_sys);
			luaL_error(L, "addPsys: Invalid mesh index %d", idx);
		}

		new_sys.mesh = warband->resource_manager.meshes[idx];
		new_sys.mesh_name = new_sys.mesh->name;
	}
	//WSE->Log.Info("found mesh");


	new_sys.flags = 0;
	lua_getfield(L, 1, "flags");
	if (lua_type(L, -1))
	{
		lua_pushnil(L);
		while (lua_next(L, -2))
		{
			new_sys.flags |= lua_tointeger(L, -1);
			lua_pop(L, 1);
		}
	}
	lua_pop(L, 1);

	lua_getfield(L, 1, "num_particles");
	new_sys.num_particles = (float)lua_tointeger(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, 1, "life");
	new_sys.life = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, 1, "damping");
	new_sys.damping = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, 1, "gravity_strength");
	new_sys.gravity_strength = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, 1, "turbulance_size");
	new_sys.turbulence_size = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, 1, "turbulance_strength");
	new_sys.turbulence_strength = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);

	// ##Keys##
	lua_getfield(L, 1, "alpha_keys");
	lToPsysKeyPair(L, -1, new_sys.alpha);
	lua_pop(L, 1);

	lua_getfield(L, 1, "red_keys");
	lToPsysKeyPair(L, -1, new_sys.red);
	lua_pop(L, 1);

	lua_getfield(L, 1, "green_keys");
	lToPsysKeyPair(L, -1, new_sys.green);
	lua_pop(L, 1);

	lua_getfield(L, 1, "blue_keys");
	lToPsysKeyPair(L, -1, new_sys.blue);
	lua_pop(L, 1);

	lua_getfield(L, 1, "scale_keys");
	lToPsysKeyPair(L, -1, new_sys.scale);
	lua_pop(L, 1);
	// ###

	lua_getfield(L, 1, "emit_box_size");
	new_sys.emit_box_size = lToVec3(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, 1, "emit_velocity");
	new_sys.emit_velocity = lToVec3(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, 1, "emit_dir_randomness");
	new_sys.emit_randomness = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, 1, "rotation_speed");
	new_sys.angular_speed = (float)lua_tonumber(L, -1) * (float)M_PI / 180.0f;
	lua_pop(L, 1);

	lua_getfield(L, 1, "rotation_damping");
	new_sys.angular_damping = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);

	int index = warband->particle_system_manager.add_system(new_sys);
	//WSE->Log.Info("done, index=%i", index);

	/*for (int i = 0; i < warband->particle_system_manager.num_particle_systems; i++)
	{
	wb::particle_system &s = warband->particle_system_manager.particle_systems[i];
	WSE->Log.Info("################################################################################");
	WSE->Log.Info("id: %s, flags: %i, mesh: %s", s.id.c_str(), s.flags, s.mesh_name.c_str());
	WSE->Log.Info("particles: %f, life: %f, damping: %f, grav: %f, turbsize: %f, trbstr: %f", s.num_particles, s.life, s.damping, s.gravity_strength, s.turbulence_size, s.turbulence_strength);

	WSE->Log.Info("alpha: %f, %f | %f %f", s.alpha[0].time, s.alpha[0].magnitude, s.alpha[1].time, s.alpha[1].magnitude);
	WSE->Log.Info("red: %f, %f | %f %f", s.red[0].time, s.red[0].magnitude, s.red[1].time, s.red[1].magnitude);
	WSE->Log.Info("green: %f, %f | %f %f", s.green[0].time, s.green[0].magnitude, s.green[1].time, s.green[1].magnitude);
	WSE->Log.Info("blue: %f, %f | %f %f", s.blue[0].time, s.blue[0].magnitude, s.blue[1].time, s.blue[1].magnitude);
	WSE->Log.Info("scale: %f, %f | %f %f", s.scale[0].time, s.scale[0].magnitude, s.scale[1].time, s.scale[1].magnitude);

	WSE->Log.Info("emit box size: %f, %f, %f", s.emit_box_size.x, s.emit_box_size.y, s.emit_box_size.z);
	WSE->Log.Info("emit velocity: %f, %f, %f", s.emit_velocity.x, s.emit_velocity.y, s.emit_velocity.z);

	WSE->Log.Info("emit rand: %f, rotspeed: %f, rotdamp: %f", s.emit_randomness, s.angular_speed, s.angular_damping);
	}*/

	lua_pushinteger(L, index);
	return 1;
#else
	return 0;
#endif
}

REG(removePsys)
int lc_removePsys(lua_State *L)
{
#if defined WARBAND
	int numArgs = checkLArgs(L, 1, 1, lNum);

	bool succ = warband->particle_system_manager.remove_system(lua_tointeger(L, 1));

	lua_pushboolean(L, succ ? 1 : 0);
	return 1;
#else
	return 0;
#endif
}

REG(getMeshId)
int lc_getMeshId(lua_State *L)
{
	checkLArgs(L, 1, 1, lNum);

	int idx = lua_tointeger(L, 1);
	if (idx < 0 || idx >= warband->resource_manager.meshes.size())
	{
		luaL_error(L, "Invalid mesh index %d", idx);
	}

	lua_pushstring(L, warband->resource_manager.meshes[idx]->name);
	return 1;
}

REG(getNumMeshes)
int lc_getNumMeshes(lua_State *L)
{
	lua_pushinteger(L, warband->resource_manager.meshes.size());
	return 1;
}

REG2(partiesIt, partiesI)
int lc_partiesIt(lua_State *L)
{
	gameIterator it;
	it.valid = true;
	it.advance = lPartiesIterAdvance;
	it.curValIsValid = lPartiesIterCurValIsValid;

	it.curVal = warband->cur_game->parties.get_first_valid_index();

	return lPushIterator(L, it);
}

REG2(agentsIt, agentsI)
int lc_agentsIt(lua_State *L)
{
	gameIterator it;
	it.valid = true;

	if (checkLArgs(L, 0, 3, lNum | lPos, lNum, lAny)) //pos, radius, use_grid
	{
		//Iteration with position and radius

		if (lua_gettop(L) < 2)
			luaL_error(L, "not enough arguments");

		if (lua_isnumber(L, 1))
		{
			int preg = lua_tointeger(L, 1);
			if (preg < 0 || preg >= NUM_REGISTERS)
				luaL_error(L, "pos index out of range");
			it.pos = warband->basic_game.position_registers[preg];
		}
		else
		{
			it.pos = lToPos(L, 1);
		}

		it.radius = (float)lua_tonumber(L, 2);
		it.positional_succ = false;
		it.curValIsValid = lAgentsIterCurValIsValid_positional;

		if (lIsTrue(L, 3)) //use_grid
		{
			it.advance = lAgentsIterAdvance_grid;

			if (warband->cur_mission->grid.initialize_iterator(it.grid_iterator, it.pos.o, it.radius))
			{
				it.curVal = it.grid_iterator.agent_obj->agent->no;
				it.positional_succ = true;
			}
		}
		else
		{
			it.advance = lAgentsIterAdvance_pos;

			it.curVal = warband->cur_mission->agents.get_first_valid_index();
			for (; it.curVal < warband->cur_mission->agents.size(); it.curVal = warband->cur_mission->agents.get_next_valid_index(it.curVal))
			{
				wb::agent *agent = &warband->cur_mission->agents[it.curVal];
				if ((it.pos.o - agent->position).length() <= it.radius){
					it.positional_succ = true;
					break;
				}
			}
		}
	}
	else
	{
		//Iterate over all agents
		it.advance = lAgentsIterAdvance;
		it.curValIsValid = lAgentsIterCurValIsValid;
		it.curVal = warband->cur_mission->agents.get_first_valid_index();
	}

	return lPushIterator(L, it);
}

REG2(propInstIt, propInstI)
int lc_propInstIt(lua_State *L)
{
	gameIterator it;
	it.valid = true;
	it.advance = lPropInstIterAdvance;
	it.curValIsValid = lPropInstIterCurValIsValid;

	it.subKindNo = 0;
	it.metaType = 0;

	int num_args = checkLArgs(L, 0, 2, lNum, lNum);
	if (num_args >= 1) it.subKindNo = lua_tointeger(L, 1);
	if (num_args >= 2) it.metaType = lua_tointeger(L, 2);

	it.curVal = warband->cur_mission->mission_objects.get_first_valid_index();
	for (; it.curVal < warband->cur_mission->mission_objects.size(); it.curVal = warband->cur_mission->mission_objects.get_next_valid_index(it.curVal))
	{
		wb::mission_object *mission_object = &warband->cur_mission->mission_objects[it.curVal];

		if ((it.subKindNo <= 0 || mission_object->sub_kind_no == it.subKindNo) && (it.metaType <= 0 || mission_object->meta_type == it.metaType - 1))
			break;
	}

	return lPushIterator(L, it);
}

REG2(playersIt, playersI)
int lc_playersIt(lua_State *L)
{
	gameIterator it;
	it.valid = true;
	it.advance = lPlayersIterAdvance;
	it.curValIsValid = lPlayersIterCurValIsValid;

	it.curVal = lIsTrue(L, 1);

	for (; it.curVal < NUM_NETWORK_PLAYERS; it.curVal++)
	{
		wb::network_player *player = &warband->multiplayer_data.players[it.curVal];

		if (player->is_active())
			break;
	}

	return lPushIterator(L, it);
}

REG(hookOperation)
int lc_hookOperation(lua_State *L)
{
	checkLArgs(L, 2, 2, lNum | lStr, lFunc | lNil);

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

		opcode = opEntry->second->opcode;
	}

	if (lua_type(L, 2) == LUA_TFUNCTION)
		WSE->LuaOperations.hookOperation(L, opcode, luaL_ref(L, LUA_REGISTRYINDEX));
	else
		WSE->LuaOperations.hookOperation(L, opcode, LUA_NOREF);

	return 0;
}

REG(unhookOperation)
int lc_unhookOperation(lua_State *L)
{
	checkLArgs(L, 1, 1, lNum | lStr);

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

		opcode = opEntry->second->opcode;
	}
	WSE->LuaOperations.hookOperation(L, opcode, LUA_NOREF);

	return 0;
}

REG(hookScript)
int lc_hookScript(lua_State *L)
{
	checkLArgs(L, 2, 2, lNum, lFunc | lNil);

	int script_no = lua_tointeger(L, 1);
	if (script_no < 0 || script_no >= warband->script_manager.num_scripts)
		luaL_error(L, "invalid script no: %d", script_no);

	if (lua_type(L, 2) == LUA_TFUNCTION)
		WSE->LuaOperations.hookScript(L, script_no, luaL_ref(L, LUA_REGISTRYINDEX));
	else
		WSE->LuaOperations.hookScript(L, script_no, LUA_NOREF);

	return 0;
}

REG(fail)
int lc_fail(lua_State *L)
{
	WSE->LuaOperations.lua_call_cfResults.back() = false;
	return 0;
}



/* GLOBAL CALLBACKS */

GREG(printStack)
int lc_printStack(lua_State *L)
{
	printStack(L);
	return 0;
}

static int luaL_getsubtable(lua_State *L, int idx, const char *fname);

GREG(_print)
int lc__print(lua_State *L)
{
	checkLArgs(L, 1, 1, lStr);

#if defined WARBAND
	warband->window_manager.display_message(lua_tostring(L, 1), 0xFFFF5555, 0);
#else
	const char *str = lua_tostring(L, 1);
	DWORD a = 0;
	WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), str, strlen(str), &a, NULL);
#endif

	return 0;
}

GREG(_log)
int lc__log(lua_State *L)
{
	checkLArgs(L, 1, 1, lStr);

	warband->log_stream.write_c_str(lua_tostring(L, 1));
	warband->log_stream.write_c_str("\n");

	return 0;
}

GREG(getTime)
int lc_getTime(lua_State *L)
{
	using namespace std::chrono;
	uint64_t t = duration_cast<milliseconds>(steady_clock::now() - WSE->LuaOperations.tStart).count();
	lua_pushinteger(L, (LUA_INTEGER)t);

	return 1;
}

void addGameConstantsToLState(lua_State *L)
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

//Creates _G.game table
//Registers all lua->C++ callbacks
//Runs lua startup code (LuaGlobals.lua)
void initLGameTable(lua_State *L)
{
	lua_newtable(L);

	//register _G.game callbacks
	for (callback_def& cb : _G_game_callbacks) {
		std::stringstream ss(cb.first);
		std::string name;

		while (std::getline(ss, name, '|')) { // '|' allows multiple names for the same callback
			lua_pushcfunction(L, cb.second);
			lua_setfield(L, -2, name.c_str());
		}
	}

	addGameConstantsToLState(L);

	lua_setglobal(L, "game");

	//register _G callbacks
	for (callback_def& cb : _G_callbacks) {
		std::stringstream ss(cb.first);
		std::string name;

		while (std::getline(ss, name, '|')) { // '|' allows multiple names for the same callback
			lua_pushcfunction(L, cb.second);
			lua_setglobal(L, name.c_str());
		}
	}

	std::string globals = load_resource_str(MAKEINTRESOURCE(IDR_LuaGlobals));
	if (luaL_dostring(L, globals.c_str()))
		printLastLuaError(L, "LuaGlobals");
}




/************ REQUIRE *************/

//Helpers for require_mobDebug
#define lua_absindex( L, idx) (((idx) >= 0 || (idx) <= LUA_REGISTRYINDEX) ? (idx) : lua_gettop(L) + (idx) +1)
static int luaL_getsubtable(lua_State *L, int idx, const char *fname)
{
	lua_getfield(L, idx, fname);
	if (lua_istable(L, -1))
		return 1;  /* table already there */
	else
	{
		lua_pop(L, 1);  /* remove previous result */
		idx = lua_absindex(L, idx);
		lua_newtable(L);
		lua_pushvalue(L, -1);  /* copy to be left at top */
		lua_setfield(L, idx, fname);  /* assign new table to field */
		return 0;  /* false, because did not find table there */
	}
}

int require_mobDebug(lua_State *L)
{
	luaopen_socket_core(L); //{socket}

	//Now write it to require "loaded" table
	luaL_getsubtable(L, LUA_REGISTRYINDEX, "_LOADED");				//	{socket},{_LOADED}
	lua_pushvalue(L, -2);											//	{socket},{_LOADED},{socket}
	lua_setfield(L, -2, "socket");  // _LOADED[modname] = module,		{socket},{_LOADED}
	lua_pop(L, 2);  // remove _LOADED table and luaopen result 			...

	std::string mob = load_resource_str(MAKEINTRESOURCE(IDR_mobDebug));

	if (luaL_dostring(L, mob.c_str()))					//{mobdebug}
		printLastLuaError(L, "mobDebug");

	//remove socket library again...
	luaL_getsubtable(L, LUA_REGISTRYINDEX, "_LOADED");	//{mobdebug},{_LOADED}
	lua_pushnil(L);										//{mobdebug},{_LOADED},nil
	lua_setfield(L, -2, "socket");						//{mobdebug},{_LOADED}
	lua_pop(L, 1);										//{mobdebug}

	gPrint("*** LUA DEBUGGER LOADED ***");

	return 1;
}

int require_luaLanes(lua_State *L)
{
	std::string lanes = load_resource_str(MAKEINTRESOURCE(IDR_LUA_LANES));
	if (luaL_dostring(L, lanes.c_str()))
		printLastLuaError(L, "lanes");

	return 1;
}

static std::vector<callback_def> libs =
{
	{ "mobDebug", require_mobDebug },
	{ "lsqlite3", luaopen_lsqlite3 },
	{ "lanes.core", luaopen_lanes_core },
	{ "lanes", require_luaLanes },
	{ "regex", luaopen_regex }
};

int wse_require_loader(lua_State *L)
{
	const char *name = luaL_checkstring(L, 1);

	for (callback_def& cb : libs)
	{
		if (strcmp(name, cb.first) == 0){
			lua_pushcfunction(L, cb.second);
			return 1;
		};
	}
	return 0;
}

//Adds our loader in package.loaders, to be used with require
void register_wse_require_loader(lua_State *L)
{
	lua_getglobal(L, "package");
	if (!lua_isnil(L, -1)){
		lua_getfield(L, -1, "loaders");
		if (!lua_isnil(L, -1)){
			int num_loaders = lua_objlen(L, -1); //{package}, {loaders}
			lua_pushnumber(L, num_loaders + 1); //{package}, {loaders}, k
			lua_pushcfunction(L, wse_require_loader); //{package}, {loaders}, k, func
			lua_settable(L, -3); //{package}, {loaders}
		}
		lua_pop(L, 1);
	}
	lua_pop(L, 1);
}