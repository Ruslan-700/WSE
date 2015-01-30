#include "WSENetworkContext.h"

#include <winhttp.h>
#include "fmod.h"
#include "WSE.h"

bool http_request(const rgl::string &url, rgl::string &response, rgl::string &user_agent, int timeout)
{
	bool error = true;
	int hostIndex = url.index_of("//");

	if (hostIndex >= 0)
		hostIndex += 2;
	else
		hostIndex = 0;

	int hostEndIndex;
	int pathIndex = url.index_of('/', hostIndex);

	if (pathIndex >= 0)
	{
		hostEndIndex = pathIndex;
		pathIndex += 1;
	}
	else
	{
		pathIndex = url.length();
		hostEndIndex = pathIndex;
	}

	wchar_t *host = url.substr(hostIndex, hostEndIndex).to_utf8();
	wchar_t *path = url.substr(pathIndex, url.length()).to_utf8();
	wchar_t *userAgent = user_agent.to_utf8();
	HINTERNET session = NULL;
	HINTERNET connect = NULL;
	HINTERNET request = NULL;
	
	session = WinHttpOpen(userAgent, WINHTTP_ACCESS_TYPE_NO_PROXY, NULL, NULL, 0);

	if (session)
		connect = WinHttpConnect(session, host, INTERNET_DEFAULT_PORT, 0);

	if (connect)
		request = WinHttpOpenRequest(connect, NULL, path, NULL, WINHTTP_NO_REFERER, NULL, 0);
	
	if (request)
	{
		WinHttpSetTimeouts(request, timeout, timeout, timeout, timeout);

		if (WinHttpSendRequest(request, NULL, 0, NULL, 0, 0, NULL) && WinHttpReceiveResponse(request, NULL))
		{
			DWORD numBytesAvailable = 0;

			do
			{
				if (WinHttpQueryDataAvailable(request, &numBytesAvailable))
				{
					error = false;

					if (numBytesAvailable > 0)
					{
						rgl::string dataRead;
						DWORD numBytesRead = 0;

						if (WinHttpReadData(request, dataRead.get_buffer(numBytesAvailable), numBytesAvailable, &numBytesRead))
						{
							dataRead.release_buffer(numBytesRead);
							response += dataRead;
						}
					}
				}
			}
			while (numBytesAvailable);
		}

		WinHttpCloseHandle(request);
	}
	
	WinHttpCloseHandle(connect);
	WinHttpCloseHandle(session);
	rgl::_delete_vec(userAgent);
	rgl::_delete_vec(path);
	rgl::_delete_vec(host);
	return error;
}

DWORD WINAPI HTTPRequestThread(LPVOID param)
{
	HTTPConnection *conn = (HTTPConnection *)param;
	
	conn->m_failed = http_request(conn->m_url, conn->m_response, conn->m_user_agent, conn->m_timeout * 1000);
	EnterCriticalSection(&WSE->Network.m_http_critical_section);
	WSE->Network.m_http_connections.push_back(conn);
	LeaveCriticalSection(&WSE->Network.m_http_critical_section);
	return EXIT_SUCCESS;
}

WSENetworkContext::WSENetworkContext()
{
	m_cur_nbuf = nullptr;
	m_ogp_server = nullptr;
	m_remote_scripting = false;
	InitializeCriticalSectionAndSpinCount(&m_http_critical_section, 1000);
}

void WSENetworkContext::OnLoad()
{
	WSE->Hooks.HookFunction(this, wb::addresses::network_server_ReceiveMessage_entry, ServerNetworkMessageReceivedHook);
#if defined WARBAND
	WSE->Hooks.HookFunction(this, wb::addresses::network_client_ReceiveMessage_entry, ClientNetworkMessageReceivedHook);
#endif

	if (WSE->SettingsIni.Bool("ogp_server", "enabled", false))
	{
		m_ogp_server = new WSEOGPServer((unsigned short)WSE->SettingsIni.Int("ogp_server", "port", 22222));

		WSE->Hooks.HookFunction(this, wb::addresses::CreateMbnetHost_entry, CreateMbnetHostHook);
		WSE->Hooks.HookFunction(this, wb::addresses::DestroyMbnetHost_entry, DestroyMbnetHostHook);
	}
	
	WSE->Hooks.HookFunction(this, wb::addresses::CheckUrlReplies_entry, CheckUrlRepliesHook);
}

void WSENetworkContext::OnUnload()
{
	delete m_ogp_server;
	m_ogp_server = nullptr;
}

void WSENetworkContext::OnEvent(WSEContext *sender, WSEEvent evt)
{
	switch (evt)
	{
	case ModuleLoad:
		if ((warband->basic_game.website.length() ^ warband->basic_game.domain.length()) != 56) // INFO: using cracked master server
		{
			WSE->Hooks.UnhookFunction(this, wb::addresses::network_server_ReceiveMessage_entry);
			WSE->Hooks.UnhookFunction(this, wb::addresses::network_client_ReceiveMessage_entry);
			WSE->Hooks.HookFunction(this, wb::addresses::network_server_ReceiveMessage_entry, ClientNetworkMessageReceivedHook);
			WSE->Hooks.HookFunction(this, wb::addresses::network_client_ReceiveMessage_entry, ServerNetworkMessageReceivedHook);
		}

		warband->network_manager.anti_cheat = 0;
		m_horse_ff = false;
		m_show_xhair = true;
		m_break_compat = !WSE->ModuleSettingsIni.Bool("", "network_compatible", true);
		m_filter_mods = WSE->ModuleSettingsIni.Bool("", "hide_other_mod_servers", false);
		m_remote_scripting = WSE->SettingsIni.Bool("remote_debugging", "enabled", false);
		WSE->Hooks.HookFunctionConditional(this, m_break_compat, wb::addresses::network_manager_PopulatePlayerInfoServerEvent_entry, NetworkManagerPopulatePlayerInfoServerEventHook);
		WSE->Hooks.HookFunctionConditional(this, m_break_compat, wb::addresses::network_manager_PopulateServerOptionsServerEvent_entry, NetworkManagerPopulateServerOptionsServerEventHook);

		if (m_break_compat && warband->network_manager.num_bits_item_kind > warband->network_manager.num_bits_scene_prop)
			warband->network_manager.num_bits_scene_prop = warband->network_manager.num_bits_item_kind;
		break;
	}
}

bool WSENetworkContext::IsNetworkCompatible() const
{
	return !m_break_compat;
}

void WSENetworkContext::ReceiveMultiplayerMessage(wb::network_buffer *nbuf, int player_no, int seq, int cur_seq)
{
	m_cur_nbuf = nbuf;
	
	int cursor = nbuf->cursor_in_bits - 4;
	unsigned short my_size = nbuf->extract_uint16(MM_SIZE_BITS);
			
	if (seq > cur_seq)
		WSE->Game.ExecuteScript(WSE_SCRIPT_MULTIPLAYER_MESSAGE_RECEIVED, 2, player_no, nbuf->extract_uint16(MM_MESSAGE_BITS));

	int cursor_diff = nbuf->cursor_in_bits - cursor;
			
	if (cursor_diff != my_size)
		nbuf->cursor_in_bits += my_size - cursor_diff;

	m_cur_nbuf = nullptr;
}

void WSENetworkContext::PopulateServerOptionsServerEvent(wb::multiplayer_event *evt)
{
	wb::network_buffer nbuf;
	
	if (m_break_compat)
		nbuf.pack_uint32(0, 1);

	nbuf.pack_uint32(warband->network_manager.control_block_dir, 1);
	nbuf.pack_uint32(warband->network_manager.ghost_mode, 2);
	nbuf.pack_uint32(warband->network_manager.combat_speed, 3);

	if (m_break_compat)
	{
		nbuf.pack_uint32(m_horse_ff, 1);
		nbuf.pack_uint32(m_show_xhair, 1);
	}

	evt->type = wb::mce_server_options;
	evt->add_buffer(nbuf);
}

bool WSENetworkContext::GetHorseFriendlyFire() const
{
	return m_horse_ff;
}

void WSENetworkContext::SetHorseFriendlyFire(bool value)
{
	if (!warband->basic_game.is_server())
		return;

	if (m_horse_ff != value)
	{
		m_horse_ff = value;
		BroadcastServerOptions();
	}
}

bool WSENetworkContext::GetShowCrosshair() const
{
	return m_show_xhair;
}

void WSENetworkContext::SetShowCrosshair(bool value)
{
	if (!warband->basic_game.is_server())
		return;

	if (m_show_xhair != value)
	{
		m_show_xhair = value;
		BroadcastServerOptions();
	}
}

void WSENetworkContext::BroadcastServerOptions()
{
	if (!warband->basic_game.is_server())
		return;
	
	wb::multiplayer_event evt;

	PopulateServerOptionsServerEvent(&evt);

	for (int i = 1; i < NUM_NETWORK_PLAYERS; ++i)
	{
		warband->multiplayer_data.players[i].send_event(evt);
	}
}

void WSENetworkContext::SendRemoteScript(wb::script *script, int num_params, int *params)
{
	wb::network_buffer nbuf;

	nbuf.pack_uint16(RemoteScriptMessage, MM_EVENT_BITS);
	nbuf.pack_int32(num_params, 5);

	for (int i = 0; i < num_params; ++i)
	{
		nbuf.pack_int32(params[i], 32);
	}

	nbuf.pack_int32(script->operations.num_operations, 12);

	for (int i = 0; i < script->operations.num_operations; ++i)
	{
		nbuf.pack_uint32(script->operations.operations[i].opcode, 12);
		nbuf.pack_uint32(script->operations.operations[i].opcode >> 30, 2);
		nbuf.pack_int32(script->operations.operations[i].num_operands, 5);

		for (int j = 0; j < script->operations.operations[i].num_operands; ++j)
		{
			nbuf.pack_uint64(script->operations.operations[i].operands[j], 32);

			if (script->operations.operations[i].operands[j] < 0)
			{
				nbuf.pack_int32(1, 1);
			}
			else
			{
				nbuf.pack_int32(0, 1);
				nbuf.pack_uint64(script->operations.operations[i].operands[j] >> 56, 7);
			}
		}
	}
	
	wb::multiplayer_event evt;

	evt.type = wb::mse_wse_event;
	evt.add_buffer(nbuf);
	warband->multiplayer_data.players[0].send_event(evt);
}

void WSENetworkContext::ReceiveRemoteScript(wb::network_buffer *nbuf, int player_no, int seq, int cur_seq)
{
	wb::operation_manager mgr;
	__int64 params[16];
	int num_params = nbuf->extract_int32(5);

	for (int i = 0; i < num_params; ++i)
	{
		params[i] = nbuf->extract_int32(32);
	}

	mgr.num_operations = nbuf->extract_int32(12);
	mgr.operations = rgl::_new<wb::operation>(mgr.num_operations);

	for (int i = 0; i < mgr.num_operations; ++i)
	{
		mgr.operations[i].opcode = nbuf->extract_uint32(12);
		mgr.operations[i].opcode |= nbuf->extract_uint32(2) << 30;
		mgr.operations[i].num_operands = nbuf->extract_int32(5);

		for (int j = 0; j < mgr.operations[i].num_operands; ++j)
		{
			mgr.operations[i].operands[j] = nbuf->extract_uint64(32);

			if (nbuf->extract_int32(1))
				mgr.operations[i].operands[j] |= (unsigned __int64)0xFFFFFFFF << 32;
			else
				mgr.operations[i].operands[j] |= nbuf->extract_uint64(7) << 56;
		}
	}
			
	if (m_remote_scripting && WSE->SettingsIni.Bool("remote_debugging", itostr(warband->multiplayer_data.players[player_no].get_unique_id()), false) && seq > cur_seq)
	{
		mgr.execute(0, 0, num_params, params);
	}
}

void WSENetworkContext::HandleHTTPReplies()
{
	EnterCriticalSection(&m_http_critical_section);
	while (!m_http_connections.empty())
	{
		HTTPConnection *conn = m_http_connections.front();
		__int64 params[16];
		int num_params = 0;

		if (conn->m_failed)
		{
			if (conn->m_failure_script_no >= 0 && conn->m_failure_script_no < warband->script_manager.num_scripts)
				warband->script_manager.scripts[conn->m_failure_script_no].execute(num_params, params);
		}
		else
		{
			if (conn->m_success_script_no >= 0 && conn->m_success_script_no < warband->script_manager.num_scripts)
			{
				if (!conn->m_raw)
				{
					int num_parts;
					rgl::string *parts = conn->m_response.split('|', num_parts);
					int register_no = 0;
					int string_register_no = 0;

					for (int j = 0; j < num_parts; ++j)
					{
						if (parts[j].is_integer())
						{
							if (register_no < NUM_REGISTERS)
								warband->basic_game.registers[register_no++] = strtol(parts[j].c_str(), NULL, 10);
						}
						else
						{
							if (string_register_no < NUM_REGISTERS)
								warband->basic_game.string_registers[string_register_no++] = parts[j];
						}
					}

					rgl::_delete_vec(parts);
					num_params = 2;
					params[0] = register_no;
					params[1] = string_register_no;
				}
				else
				{
					num_params = 0;
					warband->basic_game.string_registers[0] = conn->m_response;
				}

				warband->script_manager.scripts[conn->m_success_script_no].execute(num_params, params);
			}
		}

		delete conn;
		m_http_connections.pop_front();
	}

	LeaveCriticalSection(&m_http_critical_section);
}

void WSENetworkContext::OnCreateMbnetHost()
{
	if (m_ogp_server)
		m_ogp_server->Start();
}

void WSENetworkContext::OnDestroyMbnetHost()
{
	if (m_ogp_server)
		m_ogp_server->Stop();
}

void WSENetworkContext::OnCheckUrlReplies()
{
	HandleHTTPReplies();

	if (m_ogp_server)
		m_ogp_server->Run();
}

bool WSENetworkContext::OnClientNetworkMessageReceived(int type, int player_no, wb::network_buffer *nbuf, int seq, int cur_seq)
{
#if defined WARBAND
	switch (type)
	{
	case wb::mce_server_options:
		{
			if (!m_break_compat)
				return true;
			
			int is_wse_event = nbuf->extract_uint32(1);

			if (is_wse_event)
				break;

			int control_block_dir = nbuf->extract_uint32(1);
			int ghost_mode = nbuf->extract_uint32(2);
			int combat_speed = nbuf->extract_uint32(3);
			int horse_ff = nbuf->extract_uint32(1);
			int show_xhair = nbuf->extract_uint32(1);

			if (seq > cur_seq)
			{
				warband->network_manager.control_block_dir = control_block_dir;
				warband->network_manager.ghost_mode = ghost_mode;
				warband->network_manager.combat_speed = combat_speed;
				m_horse_ff = horse_ff != 0;
				m_show_xhair = show_xhair != 0;
			}

			return false;
		}
		break;
	case wb::mce_player_info:
		{
			if (!m_break_compat)
				return true;
			
			int player_no = nbuf->extract_int32(warband->network_manager.num_bits_player, -1);
			std::string name = nbuf->extract_string(256);
			int skin_no = nbuf->extract_uint32(4);
			int banner_no = nbuf->extract_int32(10, -1);
			unsigned __int64 face_keys_1 = nbuf->extract_uint64(64);
			unsigned __int64 face_keys_2 = nbuf->extract_uint64(64);
			unsigned __int64 face_keys_3 = nbuf->extract_uint64(64);
			unsigned __int64 face_keys_4 = nbuf->extract_uint64(64);
			int team_no = nbuf->extract_int32(warband->network_manager.num_bits_team, -1);
			int troop_no = nbuf->extract_int32(warband->network_manager.num_bits_troop, -1);

			if (seq > cur_seq)
			{
				wb::network_player *player = &warband->multiplayer_data.players[player_no];

				player->clear();
				player->set_status(wb::nps_active);
				player->name = name;
				player->troop_no = troop_no;
				player->skin_no = skin_no;
				player->banner_no = banner_no;
				player->team_no = team_no;
				player->ready = 1;
				player->face_keys.keys[0] = face_keys_1;
				player->face_keys.keys[1] = face_keys_2;
				player->face_keys.keys[2] = face_keys_3;
				player->face_keys.keys[3] = face_keys_4;
			}

			return false;
		}
	case wb::mce_server_info_steam:
	case wb::mce_server_info:
		{
			wb::multiplayer_server server;

			server.name = nbuf->extract_string(50);
			server.compatible_game_version = nbuf->extract_uint32(14);

			unsigned int compat_version = 0;

			if (server.compatible_game_version & (1 << 13))
				compat_version = (server.compatible_game_version - 1130) & ~(1 << 13);

			if (compat_version > 0)
			{
				if (!m_break_compat)
					server.compatible_game_version = 0;
				else if (compat_version < WSE_NETCODE_VERSION)
					server.compatible_game_version = 0;
				else if (compat_version > WSE_NETCODE_VERSION)
					server.compatible_game_version = 9999;
				else
					server.compatible_game_version = 1130;
			}
			else if (m_break_compat)
			{
				server.compatible_game_version = 0;
			}
						
			server.compatible_module_version = nbuf->extract_uint32(14);
			server.module_name = nbuf->extract_string(50);
			server.site_no = nbuf->extract_uint16(16);
			server.game_type_name = nbuf->extract_string(50);
			server.mission_template_no = nbuf->extract_uint16(16);
			server.map_name = nbuf->extract_string(50);
			server.players = nbuf->extract_int32(warband->network_manager.num_bits_player, -1);
			server.max_players = nbuf->extract_int32(warband->network_manager.num_bits_player, -1);
			server.passworded = nbuf->extract_uint32(1);
			server.dedicated = nbuf->extract_uint32(1);

			while (nbuf->extract_uint32(1))
			{
				server.module_settings.push_back(nbuf->extract_int32(32, 0x80000000));
			}

			nbuf->extract_float(10, 0, 0.001f);
						
			if (type == wb::mce_server_info_steam)
				server.vac_id = nbuf->extract_uint64(64);

			if (server.vac_id != 0)
				server.compatible_game_version = 0;

			wb::multiplayer_data &mpdata = warband->multiplayer_data;
			wb::network_player &player = mpdata.players[player_no];
			
			if (seq > cur_seq && mpdata.searching_servers && (!m_filter_mods || server.module_name == warband->cur_module_name))
			{
				bool found = false;

				if (mpdata.servers_temp.size())
				{
					for (int i = 0; i < mpdata.servers_temp.size(); ++i)
					{
						wb::multiplayer_server &cur_server = mpdata.servers_temp[i];

						if (cur_server.ip == inet_ntoa(player.mbnet_peer.address.ip) && cur_server.port == player.mbnet_peer.address.port)
						{
							found = true;
							break;
						}
					}
				}

				if (!found)
				{
					server.ip = inet_ntoa(player.mbnet_peer.address.ip);
					server.port = player.mbnet_peer.address.port;

					if (player.num_pings <= 0)
					{
						float offset = server.compatible_game_version < 1121 ? 0.0f : 0.2f;

						server.ping = (int)((nbuf->receive_time - player.ping_send_time - offset) * 1000.0f);
						
						if (server.ping < 0)
							server.ping = 0;
					}
					else
					{
						server.ping = player.ping_total_time / player.num_pings;
					}

					mpdata.servers_temp.push_back(server);

					if (warband->network_manager.cur_downloaded_objects > 0)
						warband->network_manager.cur_downloaded_objects--;

					for (int i = mpdata.server_list.size(); i < mpdata.servers_temp.size(); ++i)
					{
						mpdata.server_list.push_back(mpdata.servers_temp[i]);
					}
				}
			}

			mpdata.disconnected_player_nos.push_back(player_no);
			return false;
		}
	default:
		return type >= 0 && type <= 63;
	}

	if (type != wb::mce_server_options)
		return false;
	
	unsigned short my_type = nbuf->extract_uint16(MM_EVENT_BITS);
	
	switch (my_type)
	{
	case MultiplayerMessage:
		ReceiveMultiplayerMessage(nbuf, player_no, seq, cur_seq);
		break;
	case PlayerSetSkinNoClientMessage:
		{
			int player_no = nbuf->extract_int32(warband->network_manager.num_bits_player, -1);
			int skin_no = nbuf->extract_int32(4);
			wb::network_player *player = &warband->multiplayer_data.players[player_no];

			if (seq > cur_seq && player->is_active())
			{
				player->skin_no = skin_no;
			}
		}
		break;
	case PlayerStopControllingAgentMessage:
		{
			int player_no = nbuf->extract_int32(warband->network_manager.num_bits_player, -1);
			wb::network_player *player = &warband->multiplayer_data.players[player_no];

			if (seq > cur_seq && player->is_active())
			{
				if (warband->cur_mission->agents.is_valid_index(player->agent_no))
				{
					wb::agent *agent = &warband->cur_mission->agents[player->agent_no];

					agent->player_no = -1;
					agent->controller = wb::ac_bot;
				}

				player->agent_no = -1;
				player->has_agent = false;
			}
		}
		break;
	default:
		return false;
	}

#endif
	return false;
}

bool WSENetworkContext::OnServerNetworkMessageReceived(int type, int player_no, wb::network_buffer *nbuf, int seq, int cur_seq)
{
	switch (type)
	{
	case wb::mse_wse_event:
		{
			if (!m_break_compat)
				return false;
		}
		break;
	case wb::mse_server_info_request_steam:
	case wb::mse_server_info_request:
		{
			unsigned short player_slot_no = nbuf->extract_uint16(16);
			wb::basic_game &basic_game = warband->basic_game;
			wb::multiplayer_data &mpdata = warband->multiplayer_data;
			wb::network_player &player = mpdata.players[player_no];
			wb::multiplayer_event evt;
			wb::network_buffer nbuf;

			if (seq > cur_seq && player_no < NUM_NETWORK_PLAYERS)
			{
				player.mbnet_peer.u1 = player_slot_no;
				player.events.clear();
				evt.type = wb::mce_server_info_steam;
				evt.priority = 2;
				nbuf.pack_string(mpdata.server_name, 50);

				if (m_break_compat)
					nbuf.pack_uint32((1130 + WSE_NETCODE_VERSION) | (1 << 13), 14);
				else
					nbuf.pack_uint32(1130, 14);

				nbuf.pack_uint32(warband->compatible_module_version, 14);
				nbuf.pack_string(warband->cur_module_name, 50);
				nbuf.pack_uint16(mpdata.cur_site_no, 16);
				warband->cur_game->execute_script(SCRIPT_GAME_GET_SCENE_NAME, mpdata.cur_site_no);
				nbuf.pack_string(basic_game.string_registers[0], 50);
				nbuf.pack_uint16(mpdata.cur_mission_template_no, 16);
				warband->cur_game->execute_script(SCRIPT_GAME_GET_MISSION_TEMPLATE_NAME, mpdata.cur_mission_template_no);
				nbuf.pack_string(basic_game.string_registers[0], 50);
				nbuf.pack_int32(mpdata.num_active_players, warband->network_manager.num_bits_player, -1);
				nbuf.pack_int32(mpdata.max_num_private_players, warband->network_manager.num_bits_player, -1);
				nbuf.pack_uint32(mpdata.server_password.length() > 0, 1);
#if defined WARBAND
				nbuf.pack_uint32(0, 1);
#elif defined WARBAND_DEDICATED
				nbuf.pack_uint32(1, 1);
#endif

				int index = 0;
	
				do
				{
					basic_game.trigger_result = 0;
					warband->cur_game->execute_script(SCRIPT_GAME_GET_MULTIPLAYER_SERVER_OPTION_FOR_MISSION_TEMPLATE, mpdata.cur_mission_template_no, index++);

					if (basic_game.trigger_result)
					{
						nbuf.pack_uint32(1, 1);
						nbuf.pack_int32((int)basic_game.registers[0], 32);
					}
					else
					{
						nbuf.pack_int32(0, 1);
					}
				}
				while (basic_game.trigger_result);

				double time_diff = wb::functions::DXUtil_Timer(wb::TIMER_GETAPPTIME) - nbuf.receive_time;

				if (nbuf.receive_time <= 0.0 || time_diff <= 0.0)
					nbuf.pack_float(0.0f, 10, 0, 0.001f);
				else
					nbuf.pack_float(time_diff < 1.0 ? (float)time_diff : 1.0f, 10, 0, 0.001f);

				if (type == wb::mse_server_info_request_steam)
					nbuf.pack_uint64(0, 64);
	
				evt.add_buffer(nbuf);
				player.send_one_time_event(evt);
			}

			return false;
		}
	default:
		return type >= 0 && type <= 21;
	}

	if (type != wb::mse_wse_event)
		return false;
	
	unsigned short my_type = nbuf->extract_uint16(MM_EVENT_BITS);

	switch (my_type)
	{
	case MultiplayerMessage:
		ReceiveMultiplayerMessage(nbuf, player_no, seq, cur_seq);
		break;
	case RemoteScriptMessage:
		ReceiveRemoteScript(nbuf, player_no, seq, cur_seq);
		break;
	default:
		return false;
	}

	return false;
}

void WSENetworkContext::OnPopulateServerOptionsServerEvent(wb::multiplayer_event *evt)
{
	PopulateServerOptionsServerEvent(evt);
}
