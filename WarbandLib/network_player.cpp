#include "network_player.h"

#include "warband.h"

using namespace wb;

bool network_player::is_active() const
{
	return this->status == nps_active && this->ready == 1;
}
unsigned int network_player::get_unique_id() const
{
	return this->session_id & 0xFFFFFF;
}

void network_player::clear()
{
	this->agent_no = -1;
	this->troop_no = -1;
	this->ready_to_spawn = false;
	this->busy_with_menus = false;
	this->has_agent = false;
	this->gold = 0;
	this->ready = 0;
	this->entry_point_no = 0;
	this->score = 0;
	this->kills = 0;
	this->deaths = 0;
	this->team_no = 2;
	this->agent_update_movement_dirf = 0.0f;
	this->agent_update_position = rgl::vector4(0.0f);
	this->agent_update_speed = rgl::vector2(0.0f);
	this->clear_items();
	this->slots.slots.clear();
}

void network_player::clear_items()
{
	for (int i = 0; i < 9; ++i)
	{
		this->spawn_items[i] = -1;
		this->picked_up_items[i] = false;
	}
}

void network_player::set_status(network_player_status new_status)
{
	if (this->status == nps_info)
	{
		warband->multiplayer_data.num_players--;
	}
	else if (this->status == nps_active)
	{
		warband->multiplayer_data.num_players--;
		warband->multiplayer_data.num_active_players--;
	}

	this->status = new_status;

	if (new_status == nps_info)
	{
		warband->multiplayer_data.num_players++;
	}
	else if (new_status == nps_active)
	{
		warband->multiplayer_data.num_players++;
		warband->multiplayer_data.num_active_players++;
	}
}

void network_player::send_event(const multiplayer_event &evt)
{
	if (this->disconnecting)
		return;

	if (warband->basic_game.game_type == wb::gt_multi_client)
	{
		wb::multiplayer_data *data = &warband->multiplayer_data;

		if (data->my_player_no >= 0 && data->players[data->my_player_no].ready)
			this->events.push_back(evt);
	}
	else
	{
		if (this->status == wb::nps_active && this->ready)
			this->events.push_back(evt);
	}
}

void network_player::send_one_time_event(const multiplayer_event &evt)
{
	if (!this->disconnecting && (warband->basic_game.game_type == wb::gt_multi_client || this->status != wb::nps_unassigned))
	{
		this->one_time_events.push_back(evt);
		this->one_time_event_send_time = functions::DXUtil_Timer(TIMER_GETAPPTIME) + 0.2;
	}
}
