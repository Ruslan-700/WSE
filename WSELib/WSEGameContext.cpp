#include "WSEGameContext.h"

#include "WSE.h"

void WSEGameContext::OnLoad()
{
#if defined WARBAND_VANILLA
	//WSE->Hooks.HookFunction(this, wb::addresses::activate_entry, NewSerialKeyCheckHook);
#endif
	WSE->Hooks.HookFunction(this, wb::addresses::game_ReadModuleFiles_entry, GameReadModuleFilesHook);
	WSE->Hooks.HookFunction(this, wb::addresses::ReadModuleFiles_entry, ReadModuleFilesHook);
	WSE->Hooks.HookFunction(this, wb::addresses::ParseConsoleCommand_entry, ParseConsoleCommandHook);
#if defined WARBAND
	WSE->Hooks.HookFunction(this, wb::addresses::Save_entry, SaveHook);
	WSE->Hooks.HookFunction(this, wb::addresses::config_manager_ChooseNumberOfEffectiveCorpses_entry, ConfigManagerChooseNumberOfEffectiveCorpsesHook);
	WSE->Hooks.HookFunction(this, wb::addresses::game_screen_OpenWindow_entry, OpenWindowHook);
#endif
}

bool WSEGameContext::HasScript(int script_no)
{
	return m_mapped_script_nos[script_no] >= 0;
}

bool WSEGameContext::ExecuteScript(int script_no, int num_params, int param_1, int param_2, int param_3, int param_4)
{
	if (m_mapped_script_nos[script_no] < 0)
		return false;

	__int64 params[16];
	
	params[0] = param_1;
	params[1] = param_2;
	params[2] = param_3;
	params[3] = param_4;

	return warband->script_manager.scripts[m_mapped_script_nos[script_no]].execute(num_params, params);
}

void WSEGameContext::ExecuteConsoleCommand(rgl::string &message, rgl::string &command)
{
	THISCALL2(wb::addresses::ExecuteConsoleCommand_entry, this, message, command)
}

void WSEGameContext::OnReadModuleFiles()
{
	/*
	if (warband->basic_game.website.length() != 34 || warband->basic_game.domain.length() != 26) // INFO: using cracked master server
		WSE->Hooks.UnhookAll();
	*/
	
	WSE->SettingsIni.Open(WSE->GetPath() + "wse_settings.ini");
	WSE->SettingsIni.Open(std::string(warband->cur_module_path) + "\\wse_settings.ini", false);
	WSE->ModuleSettingsIni.Open(std::string(warband->cur_module_path) + "\\module.ini");
	WSE->GameVariablesIni.Open(std::string(warband->cur_module_path) + "\\game_variables.txt");

	/*
#if defined WARBAND
	if (!warband->full_version)
	{
		WSE->Log.Warning("game is running in trial mode. Disabling WSE.");
		WSE->SendContextEvent(this, ForceUnload);
		return;
	}
#endif
	*/	

#if defined WARBAND
	warband->basic_game.module_download_url = WSE->ModuleSettingsIni.String("", "module_download_url", "www.taleworlds.com/mb_warband_download_module.html");
	warband->network_manager.server.port = WSE->SettingsIni.Int("listen_server", "port", 7240);
	if (warband->network_manager.server.port < 1024 || warband->network_manager.server.port > 65535) warband->network_manager.server.port = 7240;
#endif

	char *mapped_script_ids[WSE_NUM_SCRIPTS];

	mapped_script_ids[WSE_SCRIPT_CHAT_MESSAGE_RECEIVED] = "wse_chat_message_received";
	mapped_script_ids[WSE_SCRIPT_CONSOLE_COMMAND_RECEIVED] = "wse_console_command_received";
	mapped_script_ids[WSE_SCRIPT_GAME_SAVED] = "wse_game_saved";
	mapped_script_ids[WSE_SCRIPT_MULTIPLAYER_MESSAGE_RECEIVED] = "wse_multiplayer_message_received";
	mapped_script_ids[WSE_SCRIPT_GET_AGENT_SCALE] = "wse_get_agent_scale";
	mapped_script_ids[WSE_SCRIPT_WINDOW_OPENED] = "wse_window_opened";

	for (int i = 0; i < WSE_NUM_SCRIPTS; ++i)
	{
		m_mapped_script_nos[i] = -1;
		
		for (int j = 0; j < warband->script_manager.num_scripts; ++j)
		{
			if (warband->script_manager.scripts[j].id == mapped_script_ids[i])
			{
				m_mapped_script_nos[i] = j;
				break;
			}
		}

		if (m_mapped_script_nos[i] < 0)
			WSE->Log.Warning("Unable to map WSE script code: %s", mapped_script_ids[i]);
	}
	
	for (int i = 0; i < warband->num_item_kinds; ++i)
	{
		for (int j = 0; j < warband->item_kinds[i].simple_triggers.num_simple_triggers; ++j)
		{
			warband->item_kinds[i].simple_triggers.simple_triggers[j].operations.id.format("Item Kind [%d] %s Trigger [%d]", i, warband->item_kinds[i].id.c_str(), j);
		}
	}
	
	
	for (int i = 0; i < warband->num_scene_props; ++i)
	{
		for (int j = 0; j < warband->scene_props[i].simple_triggers.num_simple_triggers; ++j)
		{
			warband->scene_props[i].simple_triggers.simple_triggers[j].operations.id.format("Scene Prop [%d] %s Trigger [%d]", i, warband->scene_props[i].id.c_str(), j);
			//rgl::_delete(warband->scene_props[i].body_part);
			//warband->scene_props[i].body_part = NULL;
		}
	}

	for (int i = 0; i < warband->conversation_manager.num_sentences; ++i)
	{
		warband->conversation_manager.sentences[i].conditions.id.format("Sentence [%d] %s Conditions", i, warband->conversation_manager.sentences[i].id.c_str());
		warband->conversation_manager.sentences[i].consequences.id.format("Sentence [%d] %s Consequences", i, warband->conversation_manager.sentences[i].id.c_str());
	}
	
	for (int i = 0; i < warband->num_map_icons; ++i)
	{
		for (int j = 0; j < warband->map_icons[i].simple_triggers.num_simple_triggers; ++j)
		{
			warband->map_icons[i].simple_triggers.simple_triggers[j].operations.id.format("Map Icon [%d] %s Trigger [%d]", i, warband->map_icons[i].id.c_str(), j);
		}
	}
	
	for (int i = 0; i < warband->script_manager.num_scripts; ++i)
	{
		warband->script_manager.scripts[i].operations.id.format("Script [%d] %s", i, warband->script_manager.scripts[i].id.c_str());
	}
	
	for (int i = 0; i < warband->num_mission_templates; ++i)
	{
		for (int j = 0; j < warband->mission_templates[i].triggers.num_triggers; ++j)
		{
			warband->mission_templates[i].triggers.triggers[j].conditions.id.format("Mission Template [%d] %s Trigger [%d] Conditions", i, warband->mission_templates[i].id.c_str(), j);
			warband->mission_templates[i].triggers.triggers[j].consequences.id.format("Mission Template [%d] %s Trigger [%d] Consequences", i, warband->mission_templates[i].id.c_str(), j);
		}
	}
	
#if defined WARBAND
	for (int i = 0; i < warband->num_menus; ++i)
	{
		warband->menus[i].operations.id.format("Menu [%d] %s", i, warband->menus[i].id.c_str());

		for (int j = 0; j < warband->menus[i].num_menu_items; ++j)
		{
			warband->menus[i].menu_items[j].conditions.id.format("Menu [%d] %s Menu Item [%d] %s Conditions", j, warband->menus[i].id.c_str(), i, warband->menus[i].menu_items[j].id.c_str());
			warband->menus[i].menu_items[j].consequences.id.format("Menu [%d] %s Menu Item [%d] %s Consequences", j, warband->menus[i].id.c_str(), i, warband->menus[i].menu_items[j].id.c_str());
		}
	}

	for (int i = 0; i < warband->presentation_manager.num_presentations; ++i)
	{
		for (int j = 0; j < warband->presentation_manager.presentations[i].simple_triggers.num_simple_triggers; ++j)
		{
			warband->presentation_manager.presentations[i].simple_triggers.simple_triggers[j].operations.id.format("Presentation [%d] %s Trigger [%d]", i, warband->presentation_manager.presentations[i].id.c_str(), j);
		}
	}

	for (int i = 0; i < warband->tableau_manager.tableau_materials.size(); ++i)
	{
		warband->tableau_manager.tableau_materials[i].operations.id.format("Tableau Material [%d] %s", i, warband->tableau_manager.tableau_materials[i].id.c_str());
	}
#endif

	WSE->SendContextEvent(this, ModuleLoad);
}

void WSEGameContext::OnReadGameFiles()
{
	for (int i = 0; i < warband->cur_game->simple_triggers.num_simple_triggers; ++i)
	{
		warband->cur_game->simple_triggers.simple_triggers[i].operations.id.format("Simple Trigger [%d]", i);
	}
	
	for (int i = 0; i < warband->cur_game->triggers.num_triggers; ++i)
	{
		warband->cur_game->triggers.triggers[i].conditions.id.format("Trigger [%d] Conditions", i);
		warband->cur_game->triggers.triggers[i].consequences.id.format("Trigger [%d] Consequences", i);
	}
	
	WSE->SendContextEvent(this, GameLoad);
}

void WSEGameContext::OnSave()
{
	ExecuteScript(WSE_SCRIPT_GAME_SAVED, 0);
}

bool WSEGameContext::OnConsoleCommandReceived(rgl::string *text, rgl::string *result)
{
	rgl::string ttext = text->trimmed();

	new (result) rgl::string();

	if (ttext.starts_with("set_max_players"))
	{
		int i1 = ttext.index_of(' ');
		int i2 = ttext.index_of(' ', i1 + 1);
		
		int max_players = strtol(&ttext.c_str()[i1], NULL, 10);
		int max_private_players = strtol(&ttext.c_str()[i2], NULL, 10);

		if (max_players < 2 || max_players > NUM_NETWORK_PLAYERS || max_private_players < 2 || max_private_players > NUM_NETWORK_PLAYERS)
		{
			*result = format("Number of players must be between %d and %d", 2, NUM_NETWORK_PLAYERS);
			return true;
		}

		if (max_players > max_private_players)
		{
			*result = "Number of players cannot exceed number of private players.";
			return true;
		}

		int limit = WSE->SettingsIni.Int("limits", "max_num_network_players", NUM_NETWORK_PLAYERS);

		if (max_private_players > limit)
		{
			*result = format("Number of players cannot exceed the %d limit set in wse_settings.ini.", limit);
			return true;
		}
		
		warband->multiplayer_data.max_num_players = max_players;
		warband->multiplayer_data.max_num_private_players = max_private_players;
		*result = "Number of players and private players are set.";
		return true;
	}

	if (!HasScript(WSE_SCRIPT_CONSOLE_COMMAND_RECEIVED))
		return false;

	warband->basic_game.string_registers[0] = *text;
	warband->basic_game.trigger_result = 0;
	warband->basic_game.result_string.clear();

	bool remote_command = false;
	if (WSE->Network.m_rcon_server && WSE->Network.m_rcon_server->m_remote_command == true)
	{
		WSE->Network.m_rcon_server->m_remote_command = false;
		remote_command = true;
	}
		
	ExecuteScript(WSE_SCRIPT_CONSOLE_COMMAND_RECEIVED, 1, remote_command);

	if (warband->basic_game.result_string.length() > 0)
		*result = warband->basic_game.result_string;
	else
		*result = *text;

	if (warband->basic_game.trigger_result)
		return true;

	return false;
}

bool WSEGameContext::OnConfigManagerChooseNumberOfEffectiveCorpses()
{
#if defined WARBAND
	if (WSE->SettingsIni.HasKey("limits", "max_corpses"))
	{
		warband->config.number_of_corpses_effective = WSE->SettingsIni.Int("limits", "max_corpses");
		return false;
	}
#endif
	return true;
}

void WSEGameContext::OnOpenWindow(int window_no)
{
#if defined WARBAND
	if ((window_no == wb::gwt_party || window_no == wb::gwt_inventory || window_no == wb::gwt_character) && HasScript(WSE_SCRIPT_WINDOW_OPENED))
	{
		wb::game_window *window = warband->game_screen.game_windows[window_no];

		warband->basic_game.trigger_result = -1;
		ExecuteScript(WSE_SCRIPT_WINDOW_OPENED, 3, window_no, window->type, window->object);

		if (warband->basic_game.trigger_result >= 0 && warband->basic_game.trigger_result < warband->presentation_manager.num_presentations)
		{
			window_no = wb::gwt_presentation;
			warband->game_screen.game_windows[wb::gwt_presentation]->type = (int)warband->basic_game.trigger_result;
			warband->game_screen.game_windows[wb::gwt_presentation]->source_window_no = window->source_window_no;
		}
	}
	
	warband->window_manager.set_tooltip_mesh(nullptr, true);
	warband->game_screen.open_windows.push_back(window_no);
	warband->game_screen.game_windows[window_no]->open();
	warband->window_manager.set_show_cursor(warband->game_screen.game_windows[window_no]->has_cursor());
#endif
}
