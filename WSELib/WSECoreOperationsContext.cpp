#include "WSECoreOperationsContext.h"

#include <ctime>
#include "WSE.h"
#include "warband.h"
#include <Shellapi.h>

bool IsVanillaWarband(WSECoreOperationsContext *context)
{
	return false;
}

void StartMapConversation(WSECoreOperationsContext *context)
{
	int troop_id, troop_dna, dialog_state;
	bool set_dialog_state;

	context->ExtractValue(troop_id);
	context->ExtractValue(troop_dna);
	context->ExtractBoolean(set_dialog_state);
	context->ExtractValue(dialog_state);

	warband->cur_game->map_conversation_troop_no = troop_id;
	warband->cur_game->map_conversation_troop_dna = troop_dna;
	warband->cur_game->map_conversation_token = set_dialog_state ? dialog_state : wb::dlg_event_triggered;
	warband->cur_game->map_conversation_start = true;
}

__int64 StoreTriggerParam(WSECoreOperationsContext *context)
{
	int index;

	context->ExtractBoundedValue(index, 1, NUM_TRIGGER_PARAMS + 1, 1);

	return WSE->Scripting.GetTriggerParam(index);
}

void ShuffleRange(WSECoreOperationsContext *context)
{
	int reg1, reg2;

	context->ExtractRegister(reg1);
	context->ExtractRegister(reg2);

	for (int i = reg1; i < reg2; ++i)
	{
		int randomRegisterNo = reg1 + rglRand(reg2 - reg1);

		std::swap(warband->basic_game.registers[i], warband->basic_game.registers[randomRegisterNo]);
	}
}

int StoreRandom(WSECoreOperationsContext *context)
{
	int upper_range;

	context->ExtractValue(upper_range);

	return rglRand(upper_range);
}

int StoreRandomInRange(WSECoreOperationsContext *context)
{
	int range_low, range_high;

	context->ExtractValue(range_low);
	context->ExtractValue(range_high);

	return range_low + rglRand(range_high - range_low);
}

__int64 RegisterGet(WSECoreOperationsContext *context)
{
	int reg;

	context->ExtractRegister(reg);

	return warband->basic_game.registers[reg];
}

void RegisterSet(WSECoreOperationsContext *context)
{
	int reg;
	__int64 value;

	context->ExtractRegister(reg);
	context->ExtractBigValue(value);

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

	while (context->HasMoreOperands() && index < NUM_RETURN_VALUES)
	{
		context->ExtractBigValue(context->m_return_values[index++]);
	}

	context->m_num_return_values = index;
}

int StoreNumReturnValues(WSECoreOperationsContext *context)
{
	return context->m_num_return_values;
}

__int64 StoreReturnValue(WSECoreOperationsContext *context)
{
	int index;

	context->ExtractValue(index, 1);

	if (index <= 0 || index > NUM_RETURN_VALUES)
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
	conn->m_post_data = "";
	conn->m_post = false;

	HANDLE hThread = CreateThread(NULL, 0, HTTPRequestThread, conn, 0, NULL);
	if (hThread) CloseHandle(hThread);
}

void SendPostMessageToUrlAdvanced(WSECoreOperationsContext *context)
{
	HTTPConnection *conn = new HTTPConnection;

	context->ExtractString(conn->m_url);
	context->ExtractString(conn->m_user_agent);
	context->ExtractString(conn->m_post_data);
	context->ExtractValue(conn->m_success_script_no, -1);
	context->ExtractValue(conn->m_failure_script_no, -1);
	context->ExtractBoolean(conn->m_raw);
	context->ExtractValue(conn->m_timeout, 10);
	conn->m_post = true;

	HANDLE hThread = CreateThread(NULL, 0, HTTPRequestThread, conn, 0, NULL);
	if (hThread) CloseHandle(hThread);
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
		time_t rawtime = (time_t)t;
		struct tm lt;

		if (localtime_s(&lt, &rawtime) == 0)
			t = (int)_mkgmtime(&lt);
	}

	return t;
}
/*
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
*/
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

	DWORD result = WaitForSingleObject((HANDLE)params[0], params[1] != 0 ? params[1] : INFINITE);
	if (result == WAIT_TIMEOUT)
		TerminateProcess((HANDLE)params[0], 0);
	CloseHandle((HANDLE)params[0]);

	delete[] params;
}

void PlayBinkFile(WSECoreOperationsContext *context)
{
#if !defined WARBAND_DEDICATED
	std::string path, cur_module_path, toreplace;
	int duration;

	context->ExtractString(path);
	context->ExtractValue(duration);

	cur_module_path = warband->cur_module_path.c_str();
	toreplace = "/";

	for (size_t start_pos = cur_module_path.find(toreplace); start_pos != std::string::npos; start_pos = cur_module_path.find(toreplace, start_pos))
	{
		cur_module_path.replace(start_pos, toreplace.length(), "\\");
	}

	char full_path[MAX_PATH];

	PathCombine(full_path, cur_module_path.c_str(), path.c_str());

	if (!PathFileExists(full_path))
		return;

	char arg_list[512];

	sprintf_s(arg_list, " \"%s\" /P /I2 /Z /J /U1 /W-1 /H-1 /C /B2", full_path);

	STARTUPINFO startup_info;
	PROCESS_INFORMATION process_info;

	ZeroMemory(&startup_info, sizeof(startup_info));
	startup_info.cb = sizeof(startup_info);
	ZeroMemory(&process_info, sizeof(process_info));

	if (CreateProcess("binkplay.exe", arg_list, NULL, NULL, false, 0, NULL, NULL, &startup_info, &process_info))
	{
		CloseHandle(process_info.hThread);

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

	if (ms >= 0)
		Sleep(ms);
}

void TimerReset(WSECoreOperationsContext *context)
{
	int timer_no;
	bool use_game_time;
	
	context->ExtractRegister(timer_no);
	context->ExtractBoolean(use_game_time);

	context->m_timer_registers[timer_no].timer_no = use_game_time ? rgl::timer_kind::app : rgl::timer_kind::mission;
	context->m_timer_registers[timer_no].update();
}

__int64 TimerGetElapsedTime(WSECoreOperationsContext *context)
{
	int timer_no;
	
	context->ExtractRegister(timer_no);

	return rglRound64((float)context->m_timer_registers[timer_no].get_elapsed_time() * 1000);
}

void ShellOpenUrl(WSECoreOperationsContext *context)
{
#if defined WARBAND
	CStringW url;

	context->ExtractWideString(url);
	
	if (url.Find(L"http://") == 0 || url.Find(L"https://") == 0 || url.Find(L"ftp://") == 0 || url.Find(L"ts3server://") == 0)
	{
		ShellExecuteW(NULL, L"open", url, NULL, NULL, SW_SHOWNORMAL);
	}
	else
	{
		context->ScriptError("Support only http://, https://, ftp:// and ts3server:// urls.");
	}
#endif
}

void SetMainParty(WSECoreOperationsContext *context)
{
	int party_no;

	context->ExtractPartyNo(party_no);

	warband->cur_game->main_party_no = party_no;
}

int GetMainParty(WSECoreOperationsContext *context)
{
	return warband->cur_game->main_party_no;
}

void MakeScreenshot(WSECoreOperationsContext *context)
{
#if defined WARBAND
	int format;
	std::string path;

	context->ExtractValue(format);
	context->ExtractPath(path);

	if (format >= 0 && format <= 3)
	{
		char *formats[4];
		formats[0] = "bmp";
		formats[1] = "jpg";
		formats[2] = "tga";
		formats[3] = "png";

		WSE->Game.ScreenShot(format, context->CreateScreenshot(path, formats[format]));
	}
	else
	{
		context->ScriptError("Support only BMP - 0, JPG - 1, TGA - 2, PNG - 3 formats.");
	}
#endif
}

void SetRandomSeed(WSECoreOperationsContext *context)
{
	int seed;

	context->ExtractValue(seed);

	rglSrand(seed);
}

__int64 StoreApplicationTime(WSECoreOperationsContext *context)
{
	return rglRound64((float)wb::functions::DXUtil_Timer(wb::TIMER_GETAPPTIME) * 1000);
}

bool IsPartySkill(WSECoreOperationsContext *context)
{
	int skill_no;

	context->ExtractSkillNo(skill_no);

	return (warband->skills[skill_no].flags & wb::sf_effects_party) > 0;
}

__int64 GetCampaignTime(WSECoreOperationsContext *context)
{
	return warband->timers[1];
}

void SetCampaignTime(WSECoreOperationsContext *context)
{
	__int64 value;

	context->ExtractBigValue(value);

	warband->timers[1] = value;

	double date = warband->cur_game->date.get_elapsed_time();
	int hour = (int)date;

	if (hour != warband->cur_game->hour)
		warband->cur_game->hour = hour;

	int day = (int)(date / 24.0f);

	if (day != warband->cur_game->day)
		warband->cur_game->day = day;

	int week = (int)(date / 168.0f);

	if (week != warband->cur_game->week)
		warband->cur_game->week = week;

	int month = (int)(date / 720.0f);

	if (month != warband->cur_game->month)
		warband->cur_game->month = month;

	int year = (int)(date / 8640.0f);

	if (year != warband->cur_game->year)
		warband->cur_game->year = year;
}

void ProfilerStart(WSECoreOperationsContext *context) 
{
	WSE->Profiling.Start();
}

void ProfilerStop(WSECoreOperationsContext *context)
{
	WSE->Profiling.Stop();
}

bool ProfilerIsRecording(WSECoreOperationsContext *context)
{
	return WSE->Profiling.IsRecording();
}

void ProfilerMark(WSECoreOperationsContext *context)
{
	rgl::string str;
	context->ExtractString(str);
	WSE->Profiling.AddMarker(str);
}

WSECoreOperationsContext::WSECoreOperationsContext() : WSEOperationContext("core", 3000, 3099)
{
	m_mersenne_twister.seed((int)time(NULL));
}

void WSECoreOperationsContext::OnLoad()
{
	/*
	DefineOperation(0, "go_to", Control, 1, 1,
		"Jump to <0>",
		"statement_no");
	*/
	DefineOperation(8, "break_loop", Control, 0, 0,
		"Break out of a loop, no matter how deeply nested in try_begin blocks");

	DefineOperation(9, "continue_loop", Control, 0, 0,
		"Continue to the next iteration of a loop, no matter how deeply nested in try_begin blocks");

	DefineOperation(12, "try_for_agents", Control | Lhs, 1, 4,
		"Loops through agents in the scene. If <1> and <2> are defined, it will only loop through agents in the chosen area. If <3> is non-zero, it will use mission grid iterator instead of searching through all agents. This is better in performance, but does not take into account the height of positions",
		"cur_agent_no", "position_no", "radius_fixed_point", "use_mission_grid");
	/*
	DefineOperation(15, "try_for_attached_parties", Control | Lhs, 2, 2,
		"Loops through parties attached to <1>.",
		"cur_party_no", "party_no");
	*/
	DefineOperation(18, "try_for_dict_keys", Control, 2, 2,
		"Loops through keys of <2>",
		"cur_key_string_register", "dict");
	
	ReplaceOperation(1004, "is_vanilla_warband", IsVanillaWarband, Both, Cf, 0, 0,
		"Fails only when WSE is running");

	ReplaceOperation(1025, "start_map_conversation", StartMapConversation, Both, None, 1, 4,
		"Starts a conversation with the selected <0>. Can be called directly from global map or game menus. <1> parameter allows you to randomize non-hero troop appearances. If <2> sets, then <3> used instead dlg_event_triggered",
		"troop_id", "troop_dna", "set_dialog_state", "dialog_state");

	DefineOperation(1025, "start_map_conversation", WSE2Extended, 1, 5,
		"Starts a conversation with the selected <0>. Can be called directly from global map or game menus. <1> parameter allows you to randomize non-hero troop appearances. If <2> sets, then <3> used instead dlg_event_triggered. If <4> sets, then conversation called directly from the presentation.",
		"troop_id", "troop_dna", "set_dialog_state", "dialog_state", "from_presentation");

	DefineOperation(1300, "start_encounter", WSE2Extended, 1, 2,
		"Forces the <1> to initiate encounter with the <0>. If <1> not specified, main party used.",
		"encountered_party_no", "party_no");

	ReplaceOperation(2070, "store_trigger_param", StoreTriggerParam, Both, Lhs | Undocumented, 1, 2,
		"Stores <1> into <0>",
		"destination", "trigger_param_no");

	ReplaceOperation(2134, "shuffle_range", ShuffleRange, Both, Undocumented, 2, 2,
		"Randomly shuffles a range of <0>..<1",
		"register_1", "register_2");

	ReplaceOperation(2135, "store_random", StoreRandom, Both, Lhs | Undocumented, 2, 2,
		"Stores a random value in the range of 0..<1>-1 into <0>",
		"destination", "upper_range");

	ReplaceOperation(2136, "store_random_in_range", StoreRandomInRange, Both, Lhs | Undocumented, 3, 3,
		"Stores a random value in the range of <1>..<2>-1 into <0>",
		"destination", "range_low", "range_high");
	
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
	/*
	RegisterOperation("close_order_menu", CloseOrderMenu, Client, None, 0, 0,
		"Closes the order menu");
	*/
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

	RegisterOperation("shell_open_url", ShellOpenUrl, Client, None, 1, 1,
		"Opens <0> in default browser. Support only http://, https://, ftp:// and ts3server:// urls.",
		"url");

	RegisterOperation("set_main_party", SetMainParty, Client, None, 1, 1,
		"Sets player's main party to <0>. Dynamic spawned party (not listed in module_parties.py) will corrupt the savegame!",
		"party_no");

	RegisterOperation("get_main_party", GetMainParty, Client, Lhs, 1, 1,
		"Stores player's main party to <0>",
		"destination");

	RegisterOperation("make_screenshot", MakeScreenshot, Client, None, 2, 2,
		"Make game screenshot. For security reasons, <1> will be saved into a Screenshots directory. Supported <0>s: BMP - 0, JPG - 1, TGA - 2, PNG - 3.",
		"format", "file");

	RegisterOperation("send_post_message_to_url_advanced", SendPostMessageToUrlAdvanced, Both, None, 3, 7,
		"Sends a HTTP POST (application/x-www-form-urlencoded) request to <0> with <1> and <2>. If the request succeeds, <3> will be called. The script will behave like game_receive_url_response, unless <5> is non-zero, in which case the script will receive no arguments and s0 will contain the full response. If the request fails, <4> will be called.",
		"url_string", "user_agent_string", "post_data", "success_callback_script_no", "failure_callback_script_no", "skip_parsing", "timeout");

	RegisterOperation("set_random_seed", SetRandomSeed, Both, None, 1, 1,
		"Seeds the random generator with <0>",
		"value");

	RegisterOperation("store_application_time", StoreApplicationTime, Both, Lhs, 1, 1,
		"Stores application time into <0> in milliseconds",
		"destination");

	RegisterOperation("is_party_skill", IsPartySkill, Both, Cf, 1, 1,
		"Fails if <0> is not effects party",
		"skill_no");

	RegisterOperation("get_campaign_time", GetCampaignTime, Client, Lhs, 1, 1,
		"Stores campaign time into <0>. 100000 = 1 game hour",
		"destination");

	RegisterOperation("set_campaign_time", SetCampaignTime, Client, None, 1, 1,
		"Sets campaign time to <0>. 100000 = 1 game hour",
		"value");

	RegisterOperation("get_mouse_map_coordinates", nullptr, Client, WSE2, 1, 1,
		"Stores mouse map coordinates into <0>",
		"position_register");
	
	RegisterOperation("profiler_start", ProfilerStart, Both, None, 0, 0, "Start the profiler");
	RegisterOperation("profiler_stop",  ProfilerStop,  Both, None, 0, 0, "Stop the profiler");
	RegisterOperation("profiler_is_recording", ProfilerIsRecording, Both, Cf, 0, 0, "Fails if profiler isn't recording");
	RegisterOperation("profiler_mark", ProfilerMark, Both, None, 1, 1,
		"Add a marker at this point in time with name <0>. Good for analyzing individual parts of a script.",
		"string_1");

	RegisterOperation("conversation_screen_auto_update_choice", nullptr, Client, WSE2, 0, 0,
		"Auto update single conversation choice without mouse click. Useful if you need to get text from external scripts.");
}
