#include "WSEOGPServer.h"

#include <cstdio>
#include <cstring>
#include <ctime>
#include "WSE.h"
#include "ogp/ogp_basictypes.h"
#include "ogp/ogp_server.h"

EOGP_Banned OGP_IsBanned(void *addr, int addr_len)
{
	return OGP_Banned_Not;
}

void OGP_UpdateServerData(ogp_serverdata_t *data)
{
	data->GameID = 0;

	data->RequestFlags.bServerInfo = 1;
	data->RequestFlags.bPlayerList = 1;
	data->RequestFlags.bTeamList = 1;
	data->RequestFlags.bAddOnList = 1;
	data->RequestFlags.bRuleList = 1;
	
	data->ServerInfo.Fields.bServerFlags = 1;
	data->ServerInfo.Fields.bConnectPort = 1;
	data->ServerInfo.Fields.bPlayerCount = 1;
	data->ServerInfo.Fields.bBotCount = 1;
	data->ServerInfo.Fields.bSlotMax = 1;
	data->ServerInfo.Fields.bReservedSlots = 1;
	data->ServerInfo.Fields.bGameName = 1;
	data->ServerInfo.Fields.bHostName = 1;
	data->ServerInfo.Fields.bMod = 1;
	data->ServerInfo.Mod.Fields.bModIdentifier = 1;
	data->ServerInfo.Fields.bMap = 1;
	data->ServerInfo.MapFields.bMapFileName = 1;
	
	data->AddOn.Fields.bAddOnFlags = 1;
	data->AddOn.Fields.bAddOnVersion = 1;
	data->AddOn.Fields.bAddOnShortName = 1;
	data->AddOn.Fields.bAddOnLongName = 1;
	
	data->Player.Fields.bPlayerFlags = 1;
	data->Player.Fields.bPlayerTeam = 1;
	data->Player.Fields.bPlayerScore = 1;
	data->Player.Fields.bPlayerKills = 1;
	data->Player.Fields.bPlayerDeath = 1;
	data->Player.Fields.bPlayerSlot = 1;
	data->Player.Fields.bPlayerPing = 1;
	data->Player.Fields.bPlayerTime = 1;
	data->Player.Fields.bPlayerName = 1;
	data->Player.Fields.bPlayerRace = 1;
	data->Player.Fields.bPlayerID = 1;
	data->Player.Fields.bPlayerClass = 1;

	data->Team.Fields.bTeamScore = 1;
	data->Team.Fields.bTeamPlayerCount = 1;
	data->Team.Fields.bTeamName = 1;
	
	EnterCriticalSection(&warband->network_manager.network_critical_section);

	if (warband->multiplayer_data.server_password.length() > 0)
		data->ServerInfo.ServerFlags.bPassword = 1;
	
#if defined WARBAND
	data->ServerInfo.ServerFlags.Type = OGP_SERVERFLAGS_TYPE_LISTEN;
#elif defined WARBAND_DEDICATED
	data->ServerInfo.ServerFlags.Type = OGP_SERVERFLAGS_TYPE_DEDICATED;
#endif
	data->ServerInfo.ServerFlags.OperatingSystem = OGP_SERVERFLAGS_OS_WINDOWS;
	data->ServerInfo.ConnectPort = warband->network_manager.server.port;
	data->ServerInfo.SlotMax = warband->multiplayer_data.max_num_private_players;
	data->ServerInfo.ReservedSlots = warband->multiplayer_data.max_num_private_players - warband->multiplayer_data.max_num_players;
	strncpy_s(data->ServerInfo.GameName, warband->basic_game.name.c_str(), 31);
	strncpy_s(data->ServerInfo.HostName, warband->multiplayer_data.server_name.c_str(), 255);
	strncpy_s(data->ServerInfo.Mod.ModName, warband->cur_module_name.c_str(), 31);
	strncpy_s(data->ServerInfo.Mod.ModIdentifier, warband->cur_module_name.c_str(), 31);
	
	if (warband->module_version > 0)
	{
		data->ServerInfo.Mod.Fields.bModVersion = 1;
		sprintf_s(data->ServerInfo.Mod.ModVersion, "%d", warband->module_version);
	}

	data->AddOn.Count = 1;
	data->AddOn.List[0].Flags.bActive = 1;
	strncpy_s(data->AddOn.List[0].Version, WSE_VERSION, 31);
	strncpy_s(data->AddOn.List[0].ShortName, "WSE", 31);
	strncpy_s(data->AddOn.List[0].LongName, "Warband Script Enhancer", 63);
	
	data->Team.Count = 4;
	data->Team.List[0].Score = warband->multiplayer_data.teams[0].score;
	data->Team.List[1].Score = warband->multiplayer_data.teams[1].score;
	data->Team.List[2].Score = warband->multiplayer_data.teams[2].score;
	data->Team.List[3].Score = warband->multiplayer_data.teams[3].score;
	data->Team.List[0].PlayerCount = 0;
	data->Team.List[1].PlayerCount = 0;
	data->Team.List[2].PlayerCount = 0;
	data->Team.List[3].PlayerCount = 0;

	int index = 0;

	for (int i = (warband->basic_game.is_dedicated_server() ? 1 : 0); i < NUM_NETWORK_PLAYERS && index < warband->multiplayer_data.num_active_players && index < OGP_PLAYERLIST_MAX; ++i)
	{
		wb::network_player *player = &warband->multiplayer_data.players[i];

		if (player->status != wb::nps_active || !player->ready)
			continue;

		strncpy_s(data->Player.List[index].Name, player->name.c_str(), 63);

		if (player->skin_no >= 0 && player->skin_no < warband->face_generator.num_skins)
			strncpy_s(data->Player.List[index].Race, warband->face_generator.skins[player->skin_no].id.c_str(), 63);

		if (player->troop_no >= 0)
			strncpy_s(data->Player.List[index].Class, warband->cur_game->troops[player->troop_no].name.c_str(), 63);

		data->Player.List[index].Score = player->score;
		data->Player.List[index].Kills = player->kills;
		data->Player.List[index].Death = player->deaths;
		data->Player.List[index].Slot = i;
		data->Player.List[index].Ping = player->ping;
		data->Player.List[index].TeamNo = player->team_no;
		data->Player.List[index].ID = player->get_unique_id();

		if (player->team_no >= 0 && player->team_no < 4)
			data->Team.List[player->team_no].PlayerCount++;

		if (player->agent_no >= 0 && player->agent_no < warband->cur_mission->agents.num_items && warband->cur_mission->agents[player->agent_no].status == wb::as_alive)
			data->Player.List[index].Flags.bAlive = 1;
		else
			data->Player.List[index].Flags.bDead = 1;

		index++;
	}
	
	data->ServerInfo.PlayerCount = index;
	data->Player.Count = index;

	data->ServerInfo.BotCount = 0;
	for (int i = 0; i < warband->cur_mission->agents.num_items; ++i)
	{
		if (warband->cur_mission->agents[i].valid)
		{
			wb::agent *agent = &warband->cur_mission->agents[i];
			if (agent->status == wb::as_alive && agent->type == wb::at_human && agent->controller == wb::ac_bot)
			{
				data->ServerInfo.BotCount++;
			}
		}
	}
	
	if (warband->multiplayer_data.cur_site_no >= 0)
	{
		warband->basic_game.string_registers[0].clear();
		warband->cur_game->execute_script(SCRIPT_GAME_GET_SCENE_NAME, warband->multiplayer_data.cur_site_no);

		if (warband->basic_game.string_registers[0].length())
			strncpy_s(data->ServerInfo.Map.MapName, warband->basic_game.string_registers[0].c_str(), 31);

		_snprintf_s(data->ServerInfo.Map.MapFileName, 32, _TRUNCATE, "%s.sco", warband->cur_game->sites[warband->multiplayer_data.cur_site_no].id.c_str());
	}
	
	if (warband->multiplayer_data.next_site_no >= 0)
	{
		warband->basic_game.string_registers[0].clear();
		warband->cur_game->execute_script(SCRIPT_GAME_GET_SCENE_NAME, warband->multiplayer_data.next_site_no);

		if (warband->basic_game.string_registers[0].length())
			strncpy_s(data->ServerInfo.NextMap.MapName, warband->basic_game.string_registers[0].c_str(), 31);

		_snprintf_s(data->ServerInfo.NextMap.MapFileName, 32, _TRUNCATE, "%s.sco", warband->cur_game->sites[warband->multiplayer_data.next_site_no].id.c_str());
	}

	if (warband->multiplayer_data.cur_mission_template_no >= 0)
	{
		warband->basic_game.string_registers[0].clear();
		warband->cur_game->execute_script(SCRIPT_GAME_GET_MISSION_TEMPLATE_NAME, warband->multiplayer_data.cur_mission_template_no);

		if (warband->basic_game.string_registers[0].length())
		{
			data->ServerInfo.Fields.bGameType = 1;
			strncpy_s(data->ServerInfo.GameType, warband->basic_game.string_registers[0].c_str(), 63);
		}
	}

	if (warband->multiplayer_data.teams[0].faction_no >= 0)
		strncpy_s(data->Team.List[0].Name, warband->cur_game->factions[warband->multiplayer_data.teams[0].faction_no].name.c_str(), 63);

	if (warband->multiplayer_data.teams[1].faction_no >= 0)
		strncpy_s(data->Team.List[1].Name, warband->cur_game->factions[warband->multiplayer_data.teams[1].faction_no].name.c_str(), 63);
	
	data->Rule.Count = 0;
	
	for (int i = 0; i < OGP_RULELIST_MAX; ++i)
	{
		warband->basic_game.trigger_result = 0;
		warband->cur_game->execute_script(SCRIPT_GAME_GET_MULTIPLAYER_SERVER_OPTION_FOR_MISSION_TEMPLATE, warband->multiplayer_data.cur_mission_template_no, i);

		if (!warband->basic_game.trigger_result)
			break;

		sprintf_s(data->Rule.List[data->Rule.Count].Value, "%lld", warband->basic_game.registers[0]);

		warband->basic_game.string_registers[0].clear();
		warband->cur_game->execute_script(SCRIPT_GAME_MULTIPLAYER_SERVER_OPTION_FOR_MISSION_TEMPLATE_TO_STRING, warband->multiplayer_data.cur_mission_template_no, i, (int)warband->basic_game.registers[0]);
		
		strncpy_s(data->Rule.List[data->Rule.Count].Key, warband->basic_game.string_registers[0].c_str(), 63);

		++data->Rule.Count;
	}
	
	LeaveCriticalSection(&warband->network_manager.network_critical_section);
}

void OGP_SendTo(const void *buffer, size_t buffer_len, void *to, int to_len, void *socket)
{
	sendto((SOCKET)socket, (char *)buffer, buffer_len, 0, (sockaddr *)to, to_len);
}

DWORD WINAPI WSEOGPServer::StartThread(LPVOID server)
{
	((WSEOGPServer *)server)->Listen();
	return 0;
}

WSEOGPServer::WSEOGPServer(unsigned short port)
{
	m_port = port;
	m_state = Inactive;
	m_update = false;
	m_thread = nullptr;
	InitializeCriticalSection(&m_cs);
}

WSEOGPServer::~WSEOGPServer()
{
	DeleteCriticalSection(&m_cs);
}

void WSEOGPServer::Start()
{
	if (m_state == Starting || m_state == Listening)
		return;

	while (m_state == Stopping)
	{
		Sleep(20);
	}

	m_state = Starting;

	sockaddr_in addr;
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		WSE->Log.Error("OGP server: error %d while starting up Winsock", WSAGetLastError());
		m_state = Inactive;
		return;
	}
	
	m_socket = socket(AF_INET, SOCK_DGRAM, 0);

	if (m_socket == INVALID_SOCKET)
	{
		WSE->Log.Error("OGP server: error %d while creating socket", WSAGetLastError());
		WSACleanup();
		m_state = Inactive;
		return;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_port);

	if (bind(m_socket, (sockaddr *)&addr, sizeof(addr)))
	{
		WSE->Log.Error("OGP server: error %d while binding socket", WSAGetLastError());
		closesocket(m_socket);
		WSACleanup();
		m_state = Inactive;
		return;
	}

	WSE->Log.Info("OGP server: listening on port %hd", m_port);
	m_thread = CreateThread(nullptr, 0, StartThread, this, 0, nullptr);
}

void WSEOGPServer::Stop()
{
	m_state = Stopping;

	EnterCriticalSection(&m_cs);
	m_update = false;
	LeaveCriticalSection(&m_cs);

	shutdown(m_socket, SD_BOTH);
	closesocket(m_socket);

	if (m_thread)
	{
		WaitForSingleObject(m_thread, 5000);
		CloseHandle(m_thread);
		m_thread = nullptr;
	}

	WSACleanup();
	WSE->Log.Info("OGP server: stopped listening");
	m_state = Inactive;
}

void WSEOGPServer::Run() // INFO: this has to be executed in the Warband thread (per-frame)
{
	EnterCriticalSection(&m_cs);

	if (!m_update)
	{
		LeaveCriticalSection(&m_cs);
		return;
	}

	char buffer[OGP_MAX_RECEIVE_SIZE];
	int recv_len = m_recv_len;

	if (recv_len > OGP_MAX_RECEIVE_SIZE)
		recv_len = OGP_MAX_RECEIVE_SIZE;

	memcpy(buffer, m_buffer, recv_len);
	sockaddr_in from = m_from;
	int from_len = m_from_len;
	m_update = false;

	LeaveCriticalSection(&m_cs);

	OGP_ReceiveFrom(buffer, recv_len, &from, from_len, (void *)m_socket);
}

void WSEOGPServer::Listen()
{
	m_state = Listening;

	char recv_buffer[OGP_MAX_RECEIVE_SIZE];
	sockaddr_in recv_from;
	int recv_from_len;
	int recv_len;

	while (m_state == Listening)
	{
		recv_from_len = sizeof(recv_from);
		recv_len = recvfrom(m_socket, recv_buffer, sizeof(recv_buffer), 0, (sockaddr *)&recv_from, &recv_from_len);

		if (recv_len <= 0)
		{
			Sleep(50);
			continue;
		}

		EnterCriticalSection(&m_cs);
		memcpy(m_buffer, recv_buffer, recv_len);
		m_recv_len = recv_len;
		m_from = recv_from;
		m_from_len = recv_from_len;
		m_update = true;
		LeaveCriticalSection(&m_cs);

		bool pending = true;

		while (pending && m_state == Listening)
		{
			Sleep(20);
			EnterCriticalSection(&m_cs);
			pending = m_update;
			LeaveCriticalSection(&m_cs);
		}
	}

	m_state = Inactive;
}
