#include "WSEPlayerOperationsContext.h"

#include "WSE.h"
#include "warband.h"

/*
void PlayerGetFaceKeys(WSEPlayerOperationsContext *context)
{
	int fkreg, player_no;

	context->ExtractRegister(fkreg);
	context->ExtractPlayerNo(player_no);

	WSE->SkinOperations.m_face_key_registers[fkreg] = warband->multiplayer_data.players[player_no].face_keys;
}

void PlayerSetFaceKeys(WSEPlayerOperationsContext *context)
{
	int player_no, fkreg;
	
	context->ExtractPlayerNo(player_no);
	context->ExtractRegister(fkreg);

	warband->multiplayer_data.players[player_no].face_keys = WSE->SkinOperations.m_face_key_registers[fkreg];
}
*/
void PlayerSetSkin(WSEPlayerOperationsContext *context)
{
	int player_no, skin_no;
	
	context->ExtractPlayerNo(player_no);
	context->ExtractSkinNo(skin_no);
	
	if (!warband->basic_game.is_server())
		return;

	wb::network_player *player = &warband->multiplayer_data.players[player_no];

	if (player->skin_no == skin_no)
		return;

	wb::network_buffer nbuf;
	
	nbuf.pack_uint16(PlayerSetSkinNoClientMessage, MM_EVENT_BITS);
	nbuf.pack_int32(player_no, warband->network_manager.num_bits_player, -1);
	nbuf.pack_uint32(skin_no, 4);

	wb::multiplayer_event evt;

	evt.type = wb::mce_server_options;

	wb::network_buffer buf;

	buf.pack_uint32(1, 1);
	evt.add_buffer(buf);
	evt.add_buffer(nbuf);

	player->skin_no = skin_no;
	
	for (int i = 1; i < NUM_NETWORK_PLAYERS; ++i)
	{
		warband->multiplayer_data.players[i].send_event(evt);
	}
}

void PlayerStopControllingAgent(WSEPlayerOperationsContext *context)
{
	int player_no;
	
	context->ExtractPlayerNo(player_no);
	
	if (!warband->basic_game.is_server())
		return;

	wb::network_player *player = &warband->multiplayer_data.players[player_no];

	if (warband->cur_mission->agents.is_valid_index(player->agent_no))
	{
		wb::agent *agent = &warband->cur_mission->agents[player->agent_no];

		agent->player_no = -1;
		agent->controller = wb::ac_bot;
		agent->ai.behavior_update_timer.update();
		agent->ai.behavior_update_time = 0.0f;
		agent->ai.movement_update_timer.update();
		agent->ai.movement_update_time = 0.0f;
		agent->ai.actions_update_timer.update();
		agent->ai.actions_update_time = 0.0f;
	}

	player->agent_no = -1;
	player->has_agent = false;

	wb::network_buffer nbuf;
	
	nbuf.pack_uint16(PlayerStopControllingAgentMessage, MM_EVENT_BITS);
	nbuf.pack_int32(player_no, warband->network_manager.num_bits_player, -1);

	wb::multiplayer_event evt;

	evt.type = wb::mce_server_options;

	wb::network_buffer buf;

	buf.pack_uint32(1, 1);
	evt.add_buffer(buf);
	evt.add_buffer(nbuf);
	
	for (int i = 1; i < NUM_NETWORK_PLAYERS; ++i)
	{
		warband->multiplayer_data.players[i].send_event(evt);
	}
}

void PlayerSetBannerId(WSEPlayerOperationsContext *context)
{
	int player_no, banner_no;
	
	context->ExtractPlayerNo(player_no);
	context->ExtractValue(banner_no);
	
	wb::network_player *player = &warband->multiplayer_data.players[player_no];

	player->banner_no = banner_no;
}

void PlayerSetUsername(WSEPlayerOperationsContext *context)
{
	int player_no;
	std::string name;
	std::string name_lowercase;

	context->ExtractPlayerNo(player_no);
	context->ExtractString(name);

	wb::network_player *player = &warband->multiplayer_data.players[player_no];

	name_lowercase = name;
	for (unsigned int i = 0; i<name_lowercase.length(); i++)
	{
		name_lowercase[i] = tolower(name_lowercase[i]);
	}

	player->name = name;
	player->name_lowercase = name_lowercase;
}

void PlayerTempBan(WSEPlayerOperationsContext *context)
{
	int player_no, banTime;

	context->ExtractValue(player_no);
	context->ExtractValue(banTime);

	if (!warband->basic_game.is_server())
		return;

	double dBanTime = (double)banTime;

	double oldGameBanTime = *((double*)wb::addresses::temp_ban_time_dbl_ptr);

	game_memcpy_s((void *)wb::addresses::temp_ban_time_dbl_ptr, (unsigned char*)&dBanTime, sizeof(double));

	std::vector<int> ops { player_no, 1 };
	WSE->Scripting.ExecuteScriptOperation(wb::ban_player, ops);

	game_memcpy_s((void *)wb::addresses::temp_ban_time_dbl_ptr, (unsigned char*)&oldGameBanTime, sizeof(double));
}

WSEPlayerOperationsContext::WSEPlayerOperationsContext() : WSEOperationContext("player", 2900, 2999)
{
}

void WSEPlayerOperationsContext::OnLoad()
{
	/*
	RegisterOperation("player_get_face_keys", PlayerGetFaceKeys, Both, None, 2, 2,
		"Stores <1>'s face keys into <0>",
		"face_key_register", "player_no");

	RegisterOperation("player_set_face_keys", PlayerSetFaceKeys, Both, None, 2, 2,
		"Sets <0>'s face keys to <1>",
		"player_no", "face_key_register");
	*/
	RegisterOperation("player_set_skin", PlayerSetSkin, Both, BreakNetwork, 2, 2,
		"Sets <0>'s skin (gender) to <1>",
		"player_no", "skin_no");

	RegisterOperation("player_stop_controlling_agent", PlayerStopControllingAgent, Both, BreakNetwork, 1, 1,
		"Gives <0>'s agent back to AI control",
		"player_no");
	
	RegisterOperation("player_set_banner_id", PlayerSetBannerId, Both, None, 2, 2,
		"Sets <0>'s banner to <1>",
		"player_no", "banner_no");

	RegisterOperation("player_set_username", PlayerSetUsername, Both, None, 2, 2,
		"Sets <0>'s username to <1>",
		"player_no", "string_no");

	RegisterOperation("player_temp_ban", PlayerTempBan, Both, None, 2, 2,
		"Bans <0> temporarily for <1> seconds",
		"player_no", "ban_time");
}
