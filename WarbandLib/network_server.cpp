#include "network_server.h"

#include "warband.h"

using namespace wb;

void network_server::broadcast_event(const multiplayer_event &evt)
{
	wb::network_player *players = warband->multiplayer_data.players;

	for (int i = 1; i < NUM_NETWORK_PLAYERS; ++i)
	{
		players[i].send_event(evt);
	}
}

void network_server::broadcast_event(const multiplayer_event &evt, int except)
{
	wb::network_player *players = warband->multiplayer_data.players;

	for (int i = 1; i < NUM_NETWORK_PLAYERS; ++i)
	{
		if (i != except)
			players[i].send_event(evt);
	}
}

bool network_server::is_team_chat_visible(const int &sender_no, const int &receiver_no) const
{
	network_player *sender = &warband->multiplayer_data.players[sender_no];
	network_player *receiver = &warband->multiplayer_data.players[receiver_no];
	mission *mission = warband->cur_mission;

	if (mission->agents.is_valid_index(sender->agent_no) && mission->agents[sender->agent_no].status == as_alive)
		return receiver->status == nps_active && receiver->ready == 1 && receiver->team_no == sender->team_no && mission->teams[receiver->team_no].relations[sender->team_no] == 1;

	if (!mission->agents.is_valid_index(receiver->agent_no) || mission->agents[receiver->agent_no].status != as_alive)
		return receiver->status == nps_active && receiver->ready == 1 && sender->team_no == receiver->team_no && mission->teams[receiver->team_no].relations[sender->team_no] == 1;
	
	return false;
}

bool network_server::is_chat_visible(const int &sender_no, const int &receiver_no) const
{
	network_player *sender = &warband->multiplayer_data.players[sender_no];
	network_player *receiver = &warband->multiplayer_data.players[receiver_no];
	mission *mission = warband->cur_mission;

	if (mission->agents.is_valid_index(sender->agent_no) && mission->agents[sender->agent_no].status == as_alive)
		return receiver->status == nps_active && receiver->ready == 1;

	if (!mission->agents.is_valid_index(receiver->agent_no) || mission->agents[receiver->agent_no].status != as_alive)
		return receiver->status == nps_active && receiver->ready == 1;
	
	return false;
}
