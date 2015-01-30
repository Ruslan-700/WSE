#include "WSEMultiplayerOperationsContext.h"

#include "WSE.h"
#include "warband.h"

void MultiplayerSendChatMessageToPlayer(WSEMultiplayerOperationsContext *context)
{
	int receiver_no, sender_no;
	bool team_chat;
	rgl::string text;
	
	context->ExtractPlayerNo(receiver_no);
	context->ExtractPlayerNo(sender_no);
	context->ExtractString(text);
	context->ExtractBoolean(team_chat);
	
	if (!warband->basic_game.is_server())
		return;

	if (receiver_no == warband->multiplayer_data.my_player_no)
	{
		wb::network_player *sender = &warband->multiplayer_data.players[sender_no];
		wb::network_player *receiver = &warband->multiplayer_data.players[receiver_no];
		rgl::string message = "";

		if (sender->team_no == 2)
		{
			message += "*";
			message += warband->language_manager.translate("ui_capital_spec");
			message += "* ";
		}
		else if (!warband->cur_mission->agents.is_valid_index(sender->agent_no) || warband->cur_mission->agents[sender->agent_no].status != wb::as_alive)
		{
			message += "*";
			message += warband->language_manager.translate("ui_capital_dead");
			message += "* ";
		}
		
		message += "[";
		message += sender->name;
		message += "] ";
		message += text;

#if defined WARBAND
		if (warband->basic_game.is_listen_server() && !warband->multiplayer_data.players[sender_no].is_muted)
		{
			if (team_chat && warband->network_manager.server.is_team_chat_visible(sender_no, receiver_no))
				warband->window_manager.display_message(message, 0xFF8888CC, 1);
			else if (warband->network_manager.server.is_chat_visible(sender_no, receiver_no))
				warband->window_manager.display_message(message, 0xFFCCCCCC, 1);
		}
#endif
	}
	else
	{
		wb::network_buffer nbuf;
		wb::multiplayer_event evt;
	
		nbuf.pack_int32(sender_no, warband->network_manager.num_bits_player, -1);
		nbuf.pack_string(text, MM_STRING_LEN);
		evt.guaranteed = 1;
		evt.type = team_chat ? wb::mce_player_team_chat : wb::mce_player_chat;
		evt.add_buffer(nbuf);

		warband->multiplayer_data.players[receiver_no].send_event(evt);
	}
}

int MultiplayerGetCurProfile(WSEMultiplayerOperationsContext *context)
{
	return warband->multiplayer_data.profile_manager.cur_profile_no;
}

int MultiplayerGetNumProfiles(WSEMultiplayerOperationsContext *context)
{
	return warband->multiplayer_data.profile_manager.profiles.size();
}

void MultiplayerProfileGetFaceKeys(WSEMultiplayerOperationsContext *context)
{
	int fkreg, profile_no;

	context->ExtractRegister(fkreg);
	context->ExtractProfileNo(profile_no);

	WSE->SkinOperations.m_face_key_registers[fkreg] = warband->multiplayer_data.profile_manager.profiles[profile_no].face_keys;
}

void MultiplayerSendCompositeMessageToPlayer(WSEMultiplayerOperationsContext *context)
{
	int player_no, message_type, nbreg;

	context->ExtractPlayerNo(player_no);
	context->ExtractBoundedValue(message_type, 0, 256);
	context->ExtractRegister(nbreg);

	wb::network_buffer *nbuf = &context->m_network_buffer_registers[nbreg];
	int old_size_in_bits = nbuf->size_in_bits;
	int old_cursor_in_bits = nbuf->cursor_in_bits;

	nbuf->cursor_in_bits = 4;
	nbuf->pack_uint16(nbuf->size_in_bits, MM_SIZE_BITS);
	nbuf->pack_uint16(message_type, MM_MESSAGE_BITS);
	nbuf->size_in_bits = old_size_in_bits;
	nbuf->cursor_in_bits = old_cursor_in_bits;
	
	if (!warband->basic_game.is_server())
		return;
	
	if (player_no == warband->multiplayer_data.my_player_no)
	{
		nbuf->cursor_in_bits = 0;
		nbuf->extract_uint16(MM_EVENT_BITS);
		WSE->Network.ReceiveMultiplayerMessage(&context->m_network_buffer_registers[nbreg], player_no);
		nbuf->cursor_in_bits = old_cursor_in_bits;
		return;
	}

	wb::multiplayer_event evt;

	evt.type = wb::mce_server_options;

	wb::network_buffer buf;

	buf.pack_uint32(1, 1);
	evt.add_buffer(buf);
	evt.add_buffer(*nbuf);
	warband->multiplayer_data.players[player_no].send_event(evt);
}

void MultiplayerSendCompositeMessageToServer(WSEMultiplayerOperationsContext *context)
{
	int message_type, nbreg;

	context->ExtractBoundedValue(message_type, 0, 256);
	context->ExtractRegister(nbreg);

	wb::network_buffer *nbuf = &context->m_network_buffer_registers[nbreg];

	int old_size_in_bits = nbuf->size_in_bits;
	int old_cursor_in_bits = nbuf->cursor_in_bits;

	nbuf->cursor_in_bits = 4;
	nbuf->pack_uint16(nbuf->size_in_bits, MM_SIZE_BITS);
	nbuf->pack_uint16(message_type, MM_MESSAGE_BITS);
	nbuf->size_in_bits = old_size_in_bits;
	nbuf->cursor_in_bits = old_cursor_in_bits;
	
	if (warband->basic_game.is_server())
	{
		nbuf->cursor_in_bits = 0;
		nbuf->extract_uint16(MM_EVENT_BITS);
		WSE->Network.ReceiveMultiplayerMessage(&context->m_network_buffer_registers[nbreg], 0);
		nbuf->cursor_in_bits = old_cursor_in_bits;
	}
	else if (warband->basic_game.game_type == wb::gt_multi_client)
	{
		wb::multiplayer_event evt;

		evt.type = wb::mse_wse_event;
		evt.add_buffer(*nbuf);
		warband->multiplayer_data.players[0].send_event(evt);
	}
}

void MultiplayerMessageInit(WSEMultiplayerOperationsContext *context)
{
	int nbreg;
	
	context->ExtractRegister(nbreg);

	wb::network_buffer *nbuf = &context->m_network_buffer_registers[nbreg];

	nbuf->~network_buffer();
	new (nbuf) wb::network_buffer;
	nbuf->pack_uint16(0, MM_EVENT_BITS);
	nbuf->pack_uint16(0, MM_SIZE_BITS);
	nbuf->pack_uint16(0, MM_MESSAGE_BITS);
}

void MultiplayerMessagePutString(WSEMultiplayerOperationsContext *context)
{
	int nbreg;
	rgl::string str;
	
	context->ExtractRegister(nbreg);
	context->ExtractString(str);

	context->m_network_buffer_registers[nbreg].pack_string(str, MM_STRING_LEN);
}

void MultiplayerMessagePutInt(WSEMultiplayerOperationsContext *context)
{
	int nbreg, value, num_bits;
	
	context->ExtractRegister(nbreg);
	context->ExtractValue(value);
	context->ExtractBoundedValue(num_bits, 1, 33, 32);

	context->m_network_buffer_registers[nbreg].pack_int32(value, num_bits);
}

void MultiplayerMessagePutPosition(WSEMultiplayerOperationsContext *context)
{
	int nbreg, preg;
	bool local;
	
	context->ExtractRegister(nbreg);
	context->ExtractRegister(preg);
	context->ExtractBoolean(local);

	rgl::matrix *pos = &warband->basic_game.position_registers[preg];
	
	if (local)
		context->m_network_buffer_registers[nbreg].pack_matrix(*pos, MM_TRANSL_LOCAL_BITS, MM_TRANSL_LOCAL_OFF, MM_TRANSL_LOCAL_MULT, MM_TRANSF_BITS, MM_TRANSF_OFF, MM_TRANSF_MULT);
	else
		context->m_network_buffer_registers[nbreg].pack_matrix(*pos, MM_TRANSL_GLOBAL_BITS, MM_TRANSL_GLOBAL_OFF, MM_TRANSL_GLOBAL_MULT, MM_TRANSF_BITS, MM_TRANSF_OFF, MM_TRANSF_MULT);
}

void MultiplayerMessagePutCoordinate(WSEMultiplayerOperationsContext *context)
{
	int nbreg, preg;
	bool local;
	
	context->ExtractRegister(nbreg);
	context->ExtractRegister(preg);
	context->ExtractBoolean(local);
	
	if (local)
		context->m_network_buffer_registers[nbreg].pack_vector4(warband->basic_game.position_registers[preg].o, MM_TRANSL_LOCAL_BITS, MM_TRANSL_LOCAL_OFF, MM_TRANSL_LOCAL_MULT);
	else
		context->m_network_buffer_registers[nbreg].pack_vector4(warband->basic_game.position_registers[preg].o, MM_TRANSL_GLOBAL_BITS, MM_TRANSL_GLOBAL_OFF, MM_TRANSL_GLOBAL_MULT);
}

void MultiplayerMessagePutFaceKeys(WSEMultiplayerOperationsContext *context)
{
	int nbreg, fkreg;
	
	context->ExtractRegister(nbreg);
	context->ExtractRegister(fkreg);

	wb::network_buffer *nbuf = &context->m_network_buffer_registers[nbreg];
	wb::face_keys *face_keys = &WSE->SkinOperations.m_face_key_registers[fkreg];
	
	nbuf->pack_uint64(face_keys->keys[0], MM_FACE_KEY_BITS);
	nbuf->pack_uint64(face_keys->keys[1], MM_FACE_KEY_BITS);
	nbuf->pack_uint64(face_keys->keys[2], MM_FACE_KEY_BITS);
}

void MultiplayerCurMessageGetString(WSEMultiplayerOperationsContext *context)
{
	int sreg;

	context->ExtractRegister(sreg);

	wb::network_buffer *nbuf = context->GetCurrentNetworkBuffer();

	nbuf->extract_string(warband->basic_game.string_registers[sreg], MM_STRING_LEN);
}

int MultiplayerCurMessageGetInt(WSEMultiplayerOperationsContext *context)
{
	int num_bits;
	
	context->ExtractBoundedValue(num_bits, 1, 33, 32);

	wb::network_buffer *nbuf = context->GetCurrentNetworkBuffer();

	return nbuf->extract_int32(num_bits);
}

void MultiplayerCurMessageGetPosition(WSEMultiplayerOperationsContext *context)
{
	int preg;
	bool local;
	
	context->ExtractRegister(preg);
	context->ExtractBoolean(local);

	wb::network_buffer *nbuf = context->GetCurrentNetworkBuffer();
	rgl::matrix *pos = &warband->basic_game.position_registers[preg];
	
	if (local)
		nbuf->extract_matrix(*pos, MM_TRANSL_LOCAL_BITS, MM_TRANSL_LOCAL_OFF, MM_TRANSL_LOCAL_MULT, MM_TRANSF_BITS, MM_TRANSF_OFF, MM_TRANSF_MULT);
	else
		nbuf->extract_matrix(*pos, MM_TRANSL_GLOBAL_BITS, MM_TRANSL_GLOBAL_OFF, MM_TRANSL_GLOBAL_MULT, MM_TRANSF_BITS, MM_TRANSF_OFF, MM_TRANSF_MULT);
}

void MultiplayerCurMessageGetCoordinate(WSEMultiplayerOperationsContext *context)
{
	int preg;
	bool local;

	context->ExtractRegister(preg);
	context->ExtractBoolean(local);

	wb::network_buffer *nbuf = context->GetCurrentNetworkBuffer();
	
	if (local)
		nbuf->extract_vector4(warband->basic_game.position_registers[preg].o, MM_TRANSL_LOCAL_BITS, MM_TRANSL_LOCAL_OFF, MM_TRANSL_LOCAL_MULT);
	else
		nbuf->extract_vector4(warband->basic_game.position_registers[preg].o, MM_TRANSL_GLOBAL_BITS, MM_TRANSL_GLOBAL_OFF, MM_TRANSL_GLOBAL_MULT);
}

void MultiplayerCurMessageGetFaceKeys(WSEMultiplayerOperationsContext *context)
{
	int fkreg;

	context->ExtractRegister(fkreg);

	wb::network_buffer *nbuf = context->GetCurrentNetworkBuffer();
	wb::face_keys *face_keys = &WSE->SkinOperations.m_face_key_registers[fkreg];
	
	face_keys->keys[0] = nbuf->extract_uint64(MM_FACE_KEY_BITS);
	face_keys->keys[1] = nbuf->extract_uint64(MM_FACE_KEY_BITS);
	face_keys->keys[2] = nbuf->extract_uint64(MM_FACE_KEY_BITS);
	face_keys->keys[3] = 0;
}

WSEMultiplayerOperationsContext::WSEMultiplayerOperationsContext() : WSEOperationContext("multiplayer", 3400, 3499)
{
}

void WSEMultiplayerOperationsContext::OnLoad()
{
	RegisterOperation("multiplayer_send_chat_message_to_player", MultiplayerSendChatMessageToPlayer, Both, None, 3, 4,
		"Sends <2> to <0> as a (native compatible) chat message by <1>. Works only on servers. <3>: 0 = chat, 1 = team chat",
		"player_no", "sender_player_no", "text", "type");
	
	RegisterOperation("multiplayer_send_composite_message_to_player", MultiplayerSendCompositeMessageToPlayer, Both, BreakNetwork, 3, 3,
		"Sends <2> with <1> to <0>",
		"player_no", "message_type", "message_register");
	
	RegisterOperation("multiplayer_send_composite_message_to_server", MultiplayerSendCompositeMessageToServer, Both, BreakNetwork, 2, 2,
		"Sends <1> with <0> to the server",
		"message_type", "message_register");
	
	RegisterOperation("multiplayer_get_cur_profile", MultiplayerGetCurProfile, Both, Lhs, 1, 1,
		"Stores the current multiplayer profile into <0>",
		"destination");
	
	RegisterOperation("multiplayer_get_num_profiles", MultiplayerGetNumProfiles, Both, Lhs, 1, 1,
		"Stores the number of multiplayer profiles into <0>",
		"destination");
	
	RegisterOperation("multiplayer_profile_get_face_keys", MultiplayerProfileGetFaceKeys, Both, None, 2, 2,
		"Stores <1>'s face keys into <0>",
		"face_key_register", "profile_no");
	
	RegisterOperation("multiplayer_message_init", MultiplayerMessageInit, Both, BreakNetwork, 1, 1,
		"Initializes (empties) <0>",
		"message_register");
	
	RegisterOperation("multiplayer_message_put_string", MultiplayerMessagePutString, Both, BreakNetwork, 2, 2,
		"Puts <1> into <0>",
		"message_register", "string");
	
	RegisterOperation("multiplayer_message_put_int", MultiplayerMessagePutInt, Both, BreakNetwork, 2, 3,
		"Puts <2> of <1> into <0>",
		"message_register", "value", "num_bits");
	
	RegisterOperation("multiplayer_message_put_position", MultiplayerMessagePutPosition, Both, BreakNetwork, 2, 3,
		"Puts <1> into <9>. Set <2> to non-zero for small, relative positions (default: scene positions)",
		"message_register", "position_register", "local");
	
	RegisterOperation("multiplayer_message_put_coordinate", MultiplayerMessagePutCoordinate, Both, BreakNetwork, 2, 3,
		"Puts x, y, z coordinates from <1> into <0>. Set <2> to non-zero for small, relative positions (default: scene positions)",
		"message_register", "position_register", "local");
	
	RegisterOperation("multiplayer_message_put_face_keys", MultiplayerMessagePutFaceKeys, Both, BreakNetwork, 2, 2,
		"Puts <1> into <0>",
		"message_register", "face_keys_register");
	
	RegisterOperation("multiplayer_cur_message_get_string", MultiplayerCurMessageGetString, Both, BreakNetwork, 1, 1,
		"Stores a string from the current message register into <0>",
		"string_register");
	
	RegisterOperation("multiplayer_cur_message_get_int", MultiplayerCurMessageGetInt, Both, Lhs|BreakNetwork, 1, 2,
		"Stores <1> of an int from the current message register into <0>. <1> MUST match the number of bits sent",
		"destination", "num_bits");
	
	RegisterOperation("multiplayer_cur_message_get_position", MultiplayerCurMessageGetPosition, Both, BreakNetwork, 1, 2,
		"Stores a position from the current message register into <0>. <1> MUST match the type sent",
		"position_register", "local");
	
	RegisterOperation("multiplayer_cur_message_get_coordinate", MultiplayerCurMessageGetCoordinate, Both, BreakNetwork, 1, 2,
		"Stores x, y, z coordinates from the current message register into <0>. <1> MUST match the type sent",
		"position_register", "local");
	
	RegisterOperation("multiplayer_cur_message_get_face_keys", MultiplayerCurMessageGetFaceKeys, Both, BreakNetwork, 1, 1,
		"Stores face keys from the current message register into <0>",
		"face_keys_register");
}

wb::network_buffer *WSEMultiplayerOperationsContext::GetCurrentNetworkBuffer()
{
	if (!WSE->Network.m_cur_nbuf)
		ScriptError("attempting to get data from uninitialized network buffer");

	return WSE->Network.m_cur_nbuf;
}
