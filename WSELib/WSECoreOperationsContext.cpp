#include "WSECoreOperationsContext.h"

#include <ctime>
#include "WSE.h"
#include "warband.h"

bool IsVanillaWarband(WSECoreOperationsContext *context)
{
	return false;
}

int StoreTriggerParam(WSECoreOperationsContext *context)
{
	int index;

	context->ExtractBoundedValue(index, 1, 16, 1);

	return WSE->Scripting.GetTriggerParam(index);
}

int RegisterGet(WSECoreOperationsContext *context)
{
	int reg;

	context->ExtractRegister(reg);

	return (int)warband->basic_game.registers[reg];
}

void RegisterSet(WSECoreOperationsContext *context)
{
	int reg, value;

	context->ExtractRegister(reg);
	context->ExtractValue(value);

	warband->basic_game.registers[reg] = value;
}

int StoreWSEVersion(WSECoreOperationsContext *context)
{
	int component;

	context->ExtractBoundedValue(component, 0, 3);

	switch (component)
	{
	case 0:
		return WSE_VERSION_MAJOR;
	case 1:
		return WSE_VERSION_MINOR;
	case 2:
		return WSE_VERSION_BUILD;
	}

	return -1;
}

bool ItemSlotGt(WSECoreOperationsContext *context)
{
	int item_kind_no, slot_no, value;
	
	context->ExtractItemKindNo(item_kind_no);
	context->ExtractValue(slot_no);
	context->ExtractValue(value);

	if (slot_no < 0 || slot_no > NUM_SLOTS)
		return 0;

	return warband->item_kinds[item_kind_no].slots.get(slot_no) > value;
}

bool PartyTemplateSlotGt(WSECoreOperationsContext *context)
{
	int party_template_no, slot_no, value;
	
	context->ExtractPartyTemplateNo(party_template_no);
	context->ExtractValue(slot_no);
	context->ExtractValue(value);

	if (slot_no < 0 || slot_no > NUM_SLOTS)
		return 0;

	return warband->party_templates[party_template_no].slots.get(slot_no) > value;
}

bool TroopSlotGt(WSECoreOperationsContext *context)
{
	int troop_no, slot_no, value;
	
	context->ExtractTroopNo(troop_no);
	context->ExtractValue(slot_no);
	context->ExtractValue(value);

	if (slot_no < 0 || slot_no > NUM_SLOTS)
		return 0;

	return warband->cur_game->troops[troop_no].slots.get(slot_no) > value;
}

bool FactionSlotGt(WSECoreOperationsContext *context)
{
	int faction_no, slot_no, value;
	
	context->ExtractFactionNo(faction_no);
	context->ExtractValue(slot_no);
	context->ExtractValue(value);

	if (slot_no < 0 || slot_no > NUM_SLOTS)
		return 0;

	return warband->cur_game->factions[faction_no].slots.get(slot_no) > value;
}

bool QuestSlotGt(WSECoreOperationsContext *context)
{
	int quest_no, slot_no, value;
	
	context->ExtractQuestNo(quest_no);
	context->ExtractValue(slot_no);
	context->ExtractValue(value);

	if (slot_no < 0 || slot_no > NUM_SLOTS)
		return 0;

	return warband->cur_game->quests[quest_no].slots.get(slot_no) > value;
}

bool SceneSlotGt(WSECoreOperationsContext *context)
{
	int site_no, slot_no, value;
	
	context->ExtractSiteNo(site_no);
	context->ExtractValue(slot_no);
	context->ExtractValue(value);

	if (slot_no < 0 || slot_no > NUM_SLOTS)
		return 0;

	return warband->cur_game->sites[site_no].slots.get(slot_no) > value;
}

bool PartySlotGt(WSECoreOperationsContext *context)
{
	int party_no, slot_no, value;
	
	context->ExtractPartyNo(party_no);
	context->ExtractValue(slot_no);
	context->ExtractValue(value);

	if (slot_no < 0 || slot_no > NUM_SLOTS)
		return 0;

	return warband->cur_game->parties[party_no].slots.get(slot_no) > value;
}

bool PlayerSlotGt(WSECoreOperationsContext *context)
{
	int player_no, slot_no, value;
	
	context->ExtractPlayerNo(player_no);
	context->ExtractValue(slot_no);
	context->ExtractValue(value);

	if (slot_no < 0 || slot_no > NUM_SLOTS)
		return 0;

	return warband->multiplayer_data.players[player_no].slots.get(slot_no) > value;
}

bool TeamSlotGt(WSECoreOperationsContext *context)
{
	int team_no, slot_no, value;
	
	context->ExtractTeamNo(team_no);
	context->ExtractValue(slot_no);
	context->ExtractValue(value);

	if (slot_no < 0 || slot_no > NUM_SLOTS)
		return 0;

	return warband->cur_mission->teams[team_no].slots.get(slot_no) > value;
}

bool AgentSlotGt(WSECoreOperationsContext *context)
{
	int agent_no, slot_no, value;
	
	context->ExtractAgentNo(agent_no);
	context->ExtractValue(slot_no);
	context->ExtractValue(value);

	if (slot_no < 0 || slot_no > NUM_SLOTS)
		return 0;

	return warband->cur_mission->agents[agent_no].slots.get(slot_no) > value;
}

bool ScenePropSlotGt(WSECoreOperationsContext *context)
{
	int mission_object_no, slot_no, value;
	
	context->ExtractMissionObjectNo(mission_object_no);
	context->ExtractValue(slot_no);
	context->ExtractValue(value);

	if (slot_no < 0 || slot_no > NUM_SLOTS)
		return 0;

	return warband->cur_mission->mission_objects[mission_object_no].slots.get(slot_no) > value;
}

int StoreCurrentTrigger(WSECoreOperationsContext *context)
{
	return context->GetCurrentTrigger();
}

void ReturnValues(WSECoreOperationsContext *context)
{
	int index = 0;

	while (context->HasMoreOperands() && index < 16)
	{
		context->ExtractValue(context->m_return_values[index++]);
	}

	context->m_num_return_values = index;
}

int StoreNumReturnValues(WSECoreOperationsContext *context)
{
	return context->m_num_return_values;
}

int StoreReturnValue(WSECoreOperationsContext *context)
{
	int index;

	context->ExtractValue(index, 1);

	if (index == 0 || index > 16)
		context->ScriptError("invalid script result index %d", index);

	if (index > context->m_num_return_values)
		context->ScriptError("unset result value %d", index);

	return context->m_return_values[index - 1];
}

void SetForcedLod(WSECoreOperationsContext *context)
{
	int level;

	context->ExtractBoundedValue(level, 0, 6);

	rgl::strategic_entity *entity = WSE->Mission.GetTriggerEntity(context->GetCurrentTrigger());

	if (entity)
		entity->flags = (entity->flags & ~0xF0) | (level << 4);
}

void SendMessageToUrlAdvanced(WSECoreOperationsContext *context)
{
	HTTPConnection *conn = new HTTPConnection;

	context->ExtractString(conn->m_url);
	context->ExtractString(conn->m_user_agent);
	context->ExtractValue(conn->m_success_script_no, -1);
	context->ExtractValue(conn->m_failure_script_no, -1);
	context->ExtractBoolean(conn->m_raw);
	context->ExtractValue(conn->m_timeout, 10);

	CreateThread(NULL, 0, HTTPRequestThread, conn, 0, NULL);
}

void Mtsrand(WSECoreOperationsContext *context)
{
	int seed;

	context->ExtractValue(seed);

	context->m_mersenne_twister.seed((unsigned int)seed);
}

int Mtrand(WSECoreOperationsContext *context)
{
	int min, max;
	
	context->ExtractValue(min);
	context->ExtractValue(max);

	std::uniform_int_distribution<int> distribution(min, max);
	
	return distribution(context->m_mersenne_twister);
}

int GetTime(WSECoreOperationsContext *context)
{
	bool local;

	context->ExtractBoolean(local);

	int t = (int)time(NULL);

	if (local)
	{
		long tz;

		_tzset();
		_get_timezone(&tz);
		t -= tz;
	}

	return t;
}

void CloseOrderMenu(WSECoreOperationsContext *context)
{
#if defined WARBAND
	if (warband->game_screen.open_windows.back() != wb::gwt_tactical)
		return;

	wb::tactical_window *tactical_window = (wb::tactical_window *)warband->game_screen.game_windows[wb::gwt_tactical];
	
	tactical_window->update_order_menu = true;
	tactical_window->order_type = -2;
	tactical_window->order_subtype = -1;

	if (tactical_window->order_flag_entity)
		tactical_window->order_flag_entity->visible = false;
	
	tactical_window->order_menu_timer.update();
#endif
}

bool OrderFlagIsActive(WSECoreOperationsContext *context)
{
#if defined WARBAND
	if (warband->game_screen.open_windows.back() != wb::gwt_tactical)
		return false;

	wb::tactical_window *tactical_window = (wb::tactical_window *)warband->game_screen.game_windows[wb::gwt_tactical];
	
	return tactical_window->order_flag_entity && tactical_window->order_flag_entity->visible;
#else
	return false;
#endif
}

void BinkThread(void *arg)
{
	DWORD *params = (DWORD *)arg;

	WaitForSingleObject((HANDLE)params[0], params[1] != 0 ? params[1] : INFINITE);
	TerminateProcess((HANDLE)params[0], 0);

	delete[] params;
}

void PlayBinkFile(WSECoreOperationsContext *context)
{
#if !defined WARBAND_DEDICATED
	std::string path;
	int duration;
	
	context->ExtractString(path);
	context->ExtractValue(duration);

	char full_path[MAX_PATH];

	PathCombine(full_path, warband->cur_module_path.c_str(), path.c_str());

	if (!PathFileExists(full_path))
		return;

	char arg_list[512];

	sprintf_s(arg_list, " %s /P /I2 /Z /J /U1 /W-1 /H-1 /C /B2", full_path);

	STARTUPINFO startup_info;
	PROCESS_INFORMATION process_info;

	ZeroMemory(&startup_info, sizeof(startup_info));
	startup_info.cb = sizeof(startup_info);
	ZeroMemory(&process_info, sizeof(process_info));

	if (CreateProcess("binkplay.exe", arg_list, NULL, NULL, false, 0, NULL, NULL, &startup_info, &process_info))
	{
		DWORD *params = new DWORD[2];
		
		params[0] = (DWORD)process_info.hProcess;
		params[1] = duration;
		_beginthread(BinkThread, 0x40000, params);
	}
#endif
}

void ProcessAdvancedUrlMessages(WSECoreOperationsContext *context)
{
	WSE->Network.HandleHTTPReplies();
}

void SleepMs(WSECoreOperationsContext *context)
{
	int ms;

	context->ExtractValue(ms);
	Sleep(ms);
}

void TimerReset(WSECoreOperationsContext *context)
{
	int timer_no;
	bool use_game_time;
	
	context->ExtractRegister(timer_no);
	context->ExtractBoolean(use_game_time);

	context->m_timer_registers[timer_no].timer_no = use_game_time ? 0 : 2;
	context->m_timer_registers[timer_no].update();
}

int TimerGetElapsedTime(WSECoreOperationsContext *context)
{
	int timer_no;
	
	context->ExtractRegister(timer_no);

	return (int)(context->m_timer_registers[timer_no].get_elapsed_time() * 1000);
}


WSECoreOperationsContext::WSECoreOperationsContext() : WSEOperationContext("core", 3000, 3099)
{
	m_mersenne_twister.seed((int)time(NULL));
}

void WSECoreOperationsContext::OnLoad()
{
	DefineOperation(0, "go_to", Control, 1, 1,
		"Jump to <0>",
		"statement_no");

	DefineOperation(8, "break_loop", Control, 0, 0,
		"Break out of a loop, no matter how deeply nested in try_begin blocks");

	DefineOperation(9, "continue_loop", Control, 0, 0,
		"Continue to the next iteration of a loop, no matter how deeply nested in try_begin blocks");

	DefineOperation(12, "try_for_agents", Control | Lhs, 1, 3,
		"Loops through agents in the scene. If <1> and <2> are defined, it will only loop through agents in the chosen area",
		"cur_agent_no", "position_no", "radius_fixed_point");

	DefineOperation(15, "try_for_attached_parties", Control | Lhs, 2, 2,
		"Loops through parties attached to <1>.",
		"cur_party_no", "party_no");

	DefineOperation(16, "try_for_active_players", Control | Lhs, 1, 2,
		"Loops through all active players. If <1> is set to non-zero, the server player (0) will be skipped",
		"cur_player_no", "skip_server");

	DefineOperation(17, "try_for_prop_instances", Control | Lhs, 1, 2,
		"Loops through scene prop instances of <1>. If <1> is not set it will loop through all scene prop instances in the scene",
		"cur_instance_no", "scene_prop_kind_no");

	DefineOperation(18, "try_for_dict_keys", Control, 2, 2,
		"Loops through keys of <2>",
		"cur_key_string_register", "dict");

	ReplaceOperation(1004, "is_vanilla_warband", IsVanillaWarband, Both, Cf, 0, 0,
		"Fails only when WSE is running");

	ReplaceOperation(2070, "store_trigger_param", StoreTriggerParam, Both, Lhs, 1, 2,
		"Stores <1> into <0>",
		"destination", "trigger_param_no");

	RegisterOperation("register_get", RegisterGet, Both, Lhs, 2, 2,
		"Stores the value of register <1> into <0>",
		"destination", "index");

	RegisterOperation("register_set", RegisterSet, Both, None, 2, 2,
		"Sets the value of register <0> to <1>",
		"index", "value");
	
	RegisterOperation("store_wse_version", StoreWSEVersion, Both, Lhs, 2, 2,
		"Stores <1> of the WSE version (0: major, 1: minor, 2: build) version into <0>",
		"destination", "component");

	RegisterOperation("item_slot_gt", ItemSlotGt, Both, Cf, 3, 3,
		"Fails if <0>'s <1> is not greater than <2>",
		"item_kind_no", "slot_no", "value");

	RegisterOperation("party_template_slot_gt", PartyTemplateSlotGt, Both, Cf, 3, 3,
		"Fails if <0>'s <1> is not greater than <2>",
		"party_template_no", "slot_no", "value");

	RegisterOperation("troop_slot_gt", TroopSlotGt, Both, Cf, 3, 3,
		"Fails if <0>'s <1> is not greater than <2>",
		"troop_no", "slot_no", "value");

	RegisterOperation("faction_slot_gt", FactionSlotGt, Both, Cf, 3, 3,
		"Fails if <0>'s <1> is not greater than <2>",
		"faction_no", "slot_no", "value");

	RegisterOperation("quest_slot_gt", QuestSlotGt, Both, Cf, 3, 3,
		"Fails if <0>'s <1> is not greater than <2>",
		"quest_no", "slot_no", "value");

	RegisterOperation("scene_slot_gt", SceneSlotGt, Both, Cf, 3, 3,
		"Fails if <0>'s <1> is not greater than <2>",
		"site_no", "slot_no", "value");

	RegisterOperation("party_slot_gt", PartySlotGt, Both, Cf, 3, 3,
		"Fails if <0>'s <1> is not greater than <2>",
		"party_no", "slot_no", "value");

	RegisterOperation("player_slot_gt", PlayerSlotGt, Both, Cf, 3, 3,
		"Fails if <0>'s <1> is not greater than <2>",
		"player_no", "slot_no", "value");

	RegisterOperation("team_slot_gt", TeamSlotGt, Both, Cf, 3, 3,
		"Fails if <0>'s <1> is not greater than <2>",
		"team_no", "slot_no", "value");

	RegisterOperation("agent_slot_gt", AgentSlotGt, Both, Cf, 3, 3,
		"Fails if <0>'s <1> is not greater than <2>",
		"agent_no", "slot_no", "value");

	RegisterOperation("scene_prop_slot_gt", ScenePropSlotGt, Both, Cf, 3, 3,
		"Fails if <0>'s <1> is not greater than <2>",
		"prop_instance_no", "slot_no", "value");

	RegisterOperation("store_current_trigger", StoreCurrentTrigger, Both, Lhs, 1, 1,
		"Stores the current trigger into <0> (0 if not in a trigger)",
		"destination");

	RegisterOperation("return_values", ReturnValues, Both, None, 0, 16,
		"Stores up to 16 return values",
		"value_1", "value_2", "value_3", "value_4", "value_5", "value_6", "value_7", "value_8", "value_9", "value_10", "value_11", "value_12", "value_13", "value_14", "value_15", "value_16");
	
	RegisterOperation("store_num_return_values", StoreNumReturnValues, Both, Lhs, 1, 1,
		"Stores the amount of return values available into <0>",
		"destination");

	RegisterOperation("store_return_value", StoreReturnValue, Both, Lhs, 1, 2,
		"Stores return value no. <1> into <0>",
		"destination", "value");

	RegisterOperation("set_forced_lod", SetForcedLod, Client, None, 1, 1,
		"Forces the current trigger entity's LOD level to <0> (0 = auto)",
		"lod_level");

	RegisterOperation("send_message_to_url_advanced", SendMessageToUrlAdvanced, Both, None, 2, 6,
		"Sends a HTTP request to <0> with <1>. If the request succeeds, <2> will be called. The script will behave like game_receive_url_response, unless <4> is non-zero, in which case the script will receive no arguments and s0 will contain the full response. If the request fails, <3> will be called.",
		"url_string", "user_agent_string", "success_callback_script_no", "failure_callback_script_no", "skip_parsing", "timeout");
	
	RegisterOperation("mtsrand", Mtsrand, Both, None, 1, 1,
		"Seeds the MT19937 random generator with <0>",
		"value");

	RegisterOperation("mtrand", Mtrand, Both, Lhs, 3, 3,
		"Stores a random value between <1> and <2> into <0> using the MT19937 random generator",
		"destination", "min", "max");
	
	RegisterOperation("get_time", GetTime, Both, Lhs, 1, 2,
		"Stores the current UNIX time into <0>. If <1> is non-zero, it will store local time instead of universal time.",
		"destination", "local");
	
	RegisterOperation("close_order_menu", CloseOrderMenu, Client, None, 0, 0,
		"Closes the order menu");
	
	RegisterOperation("order_flag_is_active", OrderFlagIsActive, Client, Cf, 0, 0,
		"Fails if the order flag is not being placed");
	
	RegisterOperation("play_bink_file", PlayBinkFile, Client, None, 1, 2,
		"Plays a .bik file located at <0>. If <1> is not set the full movie will be played",
		"path_from_module_directory", "duration_in_ms");
	
	RegisterOperation("process_advanced_url_messages", ProcessAdvancedUrlMessages, Both, None, 0, 0,
		"Forces processing of URL messages sent with send_message_to_url_advanced");
	
	RegisterOperation("sleep_ms", SleepMs, Both, None, 1, 1,
		"Sleeps (blocking the game) for <0> ms",
		"time");

	RegisterOperation("timer_reset", TimerReset, Both, None, 1, 2,
		"Resets <0>. If <1> is non-zero the timer will count game time rather than mission time",
		"timer_register_no", "use_game_time");

	RegisterOperation("timer_get_elapsed_time", TimerGetElapsedTime, Both, Lhs, 2, 2,
		"Stores <1>'s elapsed time into <0>",
		"destination", "timer_register_no");
}
