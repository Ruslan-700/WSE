#pragma once

#include "rgl.h"
#include "network_buffer.h"

namespace wb
{
	enum multiplayer_server_event_type
	{
		mse_mission_info_request = 0,
		mse_control_flags_1 = 1,
		mse_pick_up_item = 2,
		mse_drop_item = 3,
		mse_seed = 4,
		mse_player_options = 5,
		mse_message = 6,
		mse_message_1_int = 7,
		mse_message_2_int = 8,
		mse_message_3_int = 9,
		mse_message_4_int = 10,
		mse_message_string = 11,
		mse_team_chat = 12,
		mse_chat = 13,
		mse_order_position = 14,
		mse_in_menu = 15,
		mse_mission_object_use = 16,
		mse_server_join_request = 17,
		mse_server_info_request_steam = 18,
		mse_disconnect = 19,
		mse_server_info_request = 20,
		mse_site_chunk_request = 21,
		mse_wse_event = 22,
	};

	enum multiplayer_client_event_type
	{
		mce_mission_info = 0,
		mce_server_options = 1,
		mce_team_info = 2,
		mce_player_info = 3,
		mce_player_stats = 4,
		mce_agent_spawn = 5,
		mce_missile_spawn = 6,
		mce_mission_object_spawn = 7,
		mce_missile_hit_scene = 8,
		mce_missile_hit_agent = 9,
		mce_missile_hit_mission_object = 10,
		mce_missile_hit_shield = 11,
		mce_missile_hit_remove = 12,
		mce_agent_die = 13,
		mce_player_team = 14,
		mce_player_troop = 15,
		mce_player_disconnect = 16,
		mce_mission_object_remove = 17,
		mce_agent_remove = 18,
		mce_agent_fade_out = 19,
		mce_sound = 20,
		mce_agent_sound = 21,
		mce_knockback = 22,
		mce_agent_jump = 23,
		mce_agent_approximate_x = 25,
		mce_agent_approximate_y = 26,
		mce_agent_approximate_z = 27,
		mce_agent_bleed = 28,
		mce_agent_horse = 29,
		mce_agent_rider = 30,
		mce_gold = 31,
		mce_player_team_chat = 32,
		mce_player_chat = 33,
		mce_message = 34,
		mce_message_1_int = 35,
		mce_message_2_int = 36,
		mce_message_3_int = 37,
		mce_message_4_int = 38,
		mce_message_string = 39,
		mce_player_combat_message = 40,
		mce_clear_scene = 41,
		mce_mission_object_use = 42,
		mce_mission_object_animate = 43,
		mce_mission_object_animate_position = 44,
		mce_mission_object_stop_animating = 45,
		mce_mission_object_position = 46,
		mce_particle_system_burst = 47,
		mce_mission_object_hit = 48,
		mce_ranged_hit_sound = 49,
		mce_player_control_agent = 50,
		mce_server_info_steam = 53,
		mce_peer_player_no = 54,
		mce_disconnect = 55,
		mce_server_info = 56,
		mce_division_leader = 57,
		mce_agent_kick = 58,
		mce_stun = 59,
		mce_agent_fall = 60,
		mce_kick = 61,
		mce_site_chunk = 62,
		mce_join_accept_reject = 63,
	};

	struct multiplayer_event
	{
		int id;
		int u1;
		unsigned short type;
		int cursor_in_bits;
		byte buffer[508];
		int size_in_bits;
		int prev_packet_id;
		int cur_packet_id;
		int guaranteed;
		int priority;
		double time;

		multiplayer_event();
		void add_buffer(const network_buffer &nbuf);
	};
}
