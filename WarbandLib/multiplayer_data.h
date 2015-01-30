#pragma once

#include "rgl.h"
#include "agent.h"
#include "mbnet.h"
#include "multiplayer_profile.h"
#include "multiplayer_replication.h"
#include "multiplayer_server.h"
#include "network_player.h"

#define NUM_NETWORK_PLAYERS 250

namespace wb
{
	struct multiplayer_team
	{
		int kills;
		int bot_kills;
		int bot_deaths;
		int score;
		int faction_no;
	};

	struct multiplayer_data
	{
		multiplayer_profile_manager profile_manager;
		stl::vector<multiplayer_server> server_list;
		stl::vector<multiplayer_server> servers_temp;
		int num_servers;
		bool searching_servers;
		rgl::string welcome_message;
		int my_player_no;
		int num_active_players;
		int num_players;
		int max_num_players;
		int max_num_private_players;
		multiplayer_team teams[4];
		network_player players[250];
		int cur_site_no;
		int cur_seed;
		int next_server_seed;
		int started_manually;
		int cur_mission_template_no;
		int next_site_no;
		int next_seed;
		int next_mission_template_no;
		int next_team_1_faction_no;
		int next_team_2_faction_no;
		bool starting_mission;
		float health_bar_value;
		float use_time_bar_value;
		rgl::timer health_bar_display_timer;
		rgl::timer use_time_bar_display_timer;
		server_replication server_replication;
		client_replication client_relication;
		rgl::string server_name;
		rgl::string server_password_admin;
		rgl::string server_password_private;
		rgl::string server_password;
		int cur_packet_id;
		int finished_listening;
		unsigned __int64 dead_agent_update_frame_time;
		int u17;
		int u18;
		int update_agent_data;
		rgl::vector4 agent_position;
		rgl::vector2 agent_speed;
		float agent_movement_dirf;
		int u19;
		int u20;
		unsigned __int64 agent_update_frame_time;
		int u21;
		int u22;
		agent_control agent_control;
		unsigned __int64 agent_update_interval;
		stl::vector<int> disconnected_player_nos;
		stl::vector<int> banned_player_nos;
		stl::vector<void *> banned_session_ids;
		stl::vector_bool banned_statuses;
		int ban_info_player_no;
		int ban_info_player_unique_id;
		unsigned __int64 ban_info_player_session_id;
		rgl::string ban_info_player_name;
		bool ban_info_is_populated;
#if defined WARBAND_DEDICATED
		int mission_template_no;
		stl::vector<int> maps_list;
		stl::vector<int> team_1_factions_list;
		stl::vector<int> team_2_factions_list;
		int cur_map_index;
		int cur_factions_index;
		int randomize_factions;
#endif
		int u24[513];
#if defined WARBAND_STEAM
		int u25;
#endif
		mbnet_address server_address;
	};
}
