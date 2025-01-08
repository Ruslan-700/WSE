#include <chrono>
#include <math.h>
#include <sstream>
#define _USE_MATH_DEFINES

#include "WSELuaOperationsLuaCallbacks.h"
#include "WSELuaOperationsHelpers.h"
#include "WSELua_Iterators.h"
#include "luaSockets/src/luasocket.h"
#include "WSELib.rc.h"

typedef int(*lua_callback)(lua_State*);
typedef std::pair<std::string, lua_callback> callback_def; //name, callback()

std::vector<callback_def> _G_game_callbacks = {
	{ "execOperation", [](lua_State* L) -> int {
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
				luaL_error(L, "invalid operand #%d to module operation '%s'", curOperandIndex, opName.c_str());

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
	}},

	{ "getReg", [](lua_State* L) -> int {
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
	} },

	{ "setReg", [](lua_State* L) -> int {
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
	} },

	{ "getGvar", [](lua_State* L) -> int {
		checkLArgs(L, 1, 1, lStr);

		std::string gvar = lua_tostring(L, 1);

		if (WSE->LuaOperations.gvarMap.find(gvar) == WSE->LuaOperations.gvarMap.end())
			luaL_error(L, "invalid gvar '%s'", gvar);

		int idx = WSE->LuaOperations.gvarMap[gvar];
		lua_pushinteger(L, (lua_Integer)warband->basic_game.global_variables.get(idx));

		return 1;
	} },

	{ "setGvar", [](lua_State* L) -> int {
		checkLArgs(L, 2, 2, lStr, lNum);

		std::string gvar = lua_tostring(L, 1);
		lua_Integer val = lua_tointeger(L, 2);

		if (WSE->LuaOperations.gvarMap.find(gvar) == WSE->LuaOperations.gvarMap.end())
			luaL_error(L, "invalid gvar '%s'", gvar);

		int idx = WSE->LuaOperations.gvarMap[gvar];
		warband->basic_game.global_variables.set(idx, val);

		return 0;
	} },

	{ "getScriptNo", [](lua_State* L) -> int {
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
	} },

	{ "getCurTemplateNo", [](lua_State* L) -> int {
		lua_pushinteger(L, warband->cur_mission->cur_mission_template_no);
		return 1;
	} },

	{ "getCurTemplateId", [](lua_State* L) -> int {
		lua_pushstring(L, warband->mission_templates[warband->cur_mission->cur_mission_template_no].id);
		return 1;
	} },

	{ "getNumTemplates", [](lua_State* L) -> int {
		lua_pushinteger(L, warband->num_mission_templates);
		return 1;
	} },

	{ "getTemplateId", [](lua_State* L) -> int {
		checkLArgs(L, 1, 1, lNum);

		int tNo = lua_tointeger(L, 1);
		if (tNo < 0 || tNo >= warband->num_mission_templates)
			luaL_error(L, "invalid template no: %d", tNo);

		lua_pushstring(L, warband->mission_templates[tNo].id);
		return 1;
	} },

	{ "addTrigger", [](lua_State* L) -> int {
		int numArgs = checkLArgs(L, 5, 6, lStr | lNum, lNum, lNum, lNum, lFunc, lFunc);

		int tNo = lToTemplateNo(L, 1);

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
			newT.consequences.operations[0].num_operands = 3;

			newT.consequences.operations[0].operands[0] = luaL_ref(L, LUA_REGISTRYINDEX);
			newT.consequences.operations[0].operands[1] = triggerPart::consequence;
			newT.consequences.operations[0].operands[2] = (int)newT.check_interval;
		}
		else
		{
			newT.consequences.num_operations = 0;
		}

		newT.conditions.num_operations = 1;
		newT.conditions.operations = rgl::_new<wb::operation>(1);

		newT.conditions.operations[0].opcode = WSE->LuaOperations.callTriggerOpcode;
		newT.conditions.operations[0].num_operands = 3;

		newT.conditions.operations[0].operands[0] = luaL_ref(L, LUA_REGISTRYINDEX);
		newT.conditions.operations[0].operands[1] = triggerPart::condition;
		newT.conditions.operations[0].operands[2] = (int)newT.check_interval;

		int index = warband->mission_templates[tNo].addTrigger(newT, tNo, " (Lua)");

		lua_pushinteger(L, index);
		return 1;
	} },

	{ "removeTrigger", [](lua_State* L) -> int {
		int numArgs = checkLArgs(L, 2, 2, lStr | lNum, lNum);

		int tNo = lToTemplateNo(L, 1);
		int index = lua_tointeger(L, 2);

		bool succ = warband->mission_templates[tNo].removeTrigger(index);

		lua_pushboolean(L, succ ? 1 : 0);
		return 1;
	} },

	{ "getNumTriggers", [](lua_State* L) -> int {
		int numArgs = checkLArgs(L, 1, 1, lStr | lNum);

		int tNo = lToTemplateNo(L, 1);

		lua_pushinteger(L, warband->mission_templates[tNo].triggers.num_triggers);
		return 1;
	} },

	{ "addItemTrigger", [](lua_State* L) -> int {
		int numArgs = checkLArgs(L, 3, 3, lStr | lNum, lNum, lFunc);

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

		newT.operations.id.format("Item Kind [%d] %s Trigger [%d] (Lua)", itmNo, warband->item_kinds[itmNo].id.c_str(), warband->item_kinds[itmNo].simple_triggers.num_simple_triggers);
		newT.operations.num_operations = 1;
		newT.operations.operations = rgl::_new<wb::operation>(1);

		newT.operations.operations[0].opcode = WSE->LuaOperations.callTriggerOpcode;
		newT.operations.operations[0].num_operands = 3;

		newT.operations.operations[0].operands[0] = luaL_ref(L, LUA_REGISTRYINDEX);
		newT.operations.operations[0].operands[1] = triggerPart::consequence;
		newT.operations.operations[0].operands[2] = (int)newT.interval;

		int index = warband->item_kinds[itmNo].simple_triggers.addTrigger(newT);

		lua_pushinteger(L, index);
		return 1;
	} },

	{ "addScenePropTrigger", [](lua_State* L) -> int {
		int numArgs = checkLArgs(L, 3, 3, lStr | lNum, lNum, lFunc);

		int propNo;
		if (lua_type(L, 1) == LUA_TSTRING)
		{
			const char *propID = lua_tostring(L, 1);
			propNo = getScenePropNo(propID);

			if (propNo < 0)
				luaL_error(L, "invalid scene prop id: %s", propID);
		}
		else
		{
			propNo = lua_tointeger(L, 1);
			if (propNo < 0 || propNo >= warband->num_scene_props)
				luaL_error(L, "invalid scene prop no: %d", propNo);
		}

		wb::simple_trigger newT;

		newT.interval = (float)lua_tonumber(L, 2);
		newT.interval_timer = rgl::timer();

		newT.operations.id.format("Scene Prop [%d] %s Trigger [%d] (Lua)", propNo, warband->scene_props[propNo].id.c_str(), warband->scene_props[propNo].simple_triggers.num_simple_triggers);
		newT.operations.num_operations = 1;
		newT.operations.operations = rgl::_new<wb::operation>(1);

		newT.operations.operations[0].opcode = WSE->LuaOperations.callTriggerOpcode;
		newT.operations.operations[0].num_operands = 3;

		newT.operations.operations[0].operands[0] = luaL_ref(L, LUA_REGISTRYINDEX);
		newT.operations.operations[0].operands[1] = triggerPart::consequence;
		newT.operations.operations[0].operands[2] = (int)newT.interval;

		int index = warband->scene_props[propNo].simple_triggers.addTrigger(newT);

		lua_pushinteger(L, index);
		return 1;
	} },

	{ "addPrsnt", [](lua_State* L) -> int {
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

			curTrigger.interval = (float)lua_tonumber(L, -2);
			curTrigger.interval_timer = rgl::timer(2);

			curTrigger.operations.num_operations = 1;
			curTrigger.operations.operations = rgl::_new<wb::operation>(1);
			curTrigger.operations.operations[0].opcode = WSE->LuaOperations.callTriggerOpcode;
			curTrigger.operations.operations[0].num_operands = 3;
			curTrigger.operations.operations[0].operands[0] = luaL_ref(L, LUA_REGISTRYINDEX); //pops val
			curTrigger.operations.operations[0].operands[1] = triggerPart::consequence;
			curTrigger.operations.operations[0].operands[2] = (int)curTrigger.interval;

			i++;
		}
		lua_pop(L, 1);

		int index = warband->presentation_manager.addPresentation(newP);
		lua_pushinteger(L, index);
		return 1;
#else
		return 0;
#endif
	} },

	{ "removePrsnt", [](lua_State* L) -> int {
#if defined WARBAND
		int numArgs = checkLArgs(L, 1, 1, lNum);

		bool succ = warband->presentation_manager.removePresentation(lua_tointeger(L, 1));

		lua_pushboolean(L, succ ? 1 : 0);
		return 1;
#else
		return 0;
#endif
	} },

	{ "addPsys", [](lua_State* L) -> int {
#if defined WARBAND
		//WSE->Log.Info("psys check format...");
		checkTableStructure(L, 1,
			"{id=str, [flags]={(val=num)}, mesh=str,\
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

		lua_getfield(L, 1, "mesh");
		new_sys.mesh_name.initialize();
		new_sys.mesh_name = lua_tostring(L, -1);
		lua_pop(L, 1);

		new_sys.mesh = warband->resource_manager.try_get_mesh(new_sys.mesh_name, WSE->ModuleSettingsIni.Bool("", "use_case_insensitive_mesh_searches"));

		if (new_sys.mesh == nullptr){
			rgl::string name = new_sys.mesh_name;
			rgl::_free((void*)&new_sys);
			luaL_error(L, "addPsys: Could not find mesh %s", name.c_str());
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
	} },

	{ "removePsys", [](lua_State* L) -> int {
#if defined WARBAND
		int numArgs = checkLArgs(L, 1, 1, lNum);

		bool succ = warband->particle_system_manager.remove_system(lua_tointeger(L, 1));

		lua_pushboolean(L, succ ? 1 : 0);
		return 1;
#else
		return 0;
#endif
	} },

	{ "partiesI|partiesIt", [](lua_State* L) -> int {
		gameIterator it;
		it.valid = true;
		it.advance = lPartiesIterAdvance;
		it.curValIsValid = lPartiesIterCurValIsValid;

		it.curVal = warband->cur_game->parties.get_first_valid_index();

		return lPushIterator(L, it);
	} },

	{ "agentsI|agentsIt", [](lua_State* L) -> int {
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
	} },

	{ "propInstI|propInstIt", [](lua_State* L) -> int {
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
	} },

	{ "playersI|playersIt", [](lua_State* L) -> int {
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
	} },

	{ "hookOperation", [](lua_State* L) -> int {
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
	} },

	{ "unhookOperation", [](lua_State* L) -> int {
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
	} },

	{ "hookScript", [](lua_State* L) -> int {
		checkLArgs(L, 2, 2, lNum, lFunc | lNil);

		int script_no = lua_tointeger(L, 1);
		if (script_no < 0 || script_no >= warband->script_manager.num_scripts)
			luaL_error(L, "invalid script no: %d", script_no);

		if (lua_type(L, 2) == LUA_TFUNCTION)
			WSE->LuaOperations.hookScript(L, script_no, luaL_ref(L, LUA_REGISTRYINDEX));
		else
			WSE->LuaOperations.hookScript(L, script_no, LUA_NOREF);

		return 0;
	} },

	{ "fail", [](lua_State* L) -> int {
		WSE->LuaOperations.lua_call_cfResults.back() = false;
		return 0;
	} },

	{ "printStack", [](lua_State* L) -> int {
		printStack(L);
		return 0;
	} },
};

static int luaL_getsubtable(lua_State *L, int idx, const char *fname);

std::vector<callback_def> _G_callbacks = {
	{ "_print", [](lua_State* L) -> int {
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
	} },

	{ "getTime", [](lua_State* L) -> int {
		using namespace std::chrono;
		uint64_t t = duration_cast<milliseconds>(steady_clock::now() - WSE->LuaOperations.tStart).count();
		lua_pushinteger(L, (LUA_INTEGER)t);

		return 1;
	} },

	{ "loadDebugger", [](lua_State* L) -> int {
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
	} },
};

//Helpers for LoadDebugger
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