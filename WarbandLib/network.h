#pragma once

#include "rgl.h"
#include "mbnet_host.h"
#include "multiplayer_event.h"
#include "network_client.h"
#include "network_server.h"

namespace wb
{
	struct server_info_request
	{
		bool in_use;
		sockaddr address;
		SOCKET socket;
		fd_set set;
		int len;
		double connection_time;
	};

	struct network_manager
	{
		int ghost_mode;
		float ghost_mode_death_change_delay;
		int friendly_fire;
		int friendly_fire_damage_friend_ratio;
		int friendly_fire_damage_self_ratio;
		int control_block_dir;
		int add_to_game_servers_list;
		int combat_speed;
		int renaming_server_allowed;
		int changing_game_type_allowed;
		int melee_friendly_fire;
		int anti_cheat;
		int u1; //1173
		int u2; //1173
		int num_bits_player;
		int num_bits_team;
		int num_bits_faction;
		int num_bits_troop;
		int num_bits_item_kind;
		int num_bits_scene_prop;
		int num_bits_sound;
		int num_bits_num_unconfirmed_received_packet_ids;
		bool winsock_started;
		bool network_active;
		RTL_CRITICAL_SECTION network_critical_section;
		RTL_CRITICAL_SECTION url_reply_critical_section;
		mbnet_host *mbnet_host;
		int error_code;
		int action_code;
		int cur_downloaded_objects;
		int prev_downloaded_objects;
		HANDLE network_thread;
		network_server server;
#if defined WARBAND
		network_client client;
#endif
		double last_traffic_check_time;
		int cur_event_id;
		int num_bytes_sent;
		int num_bytes_received;
		int upload_bps;
		int download_bps;
		server_info_request server_info_requests[10];
		int num_server_info_requests;
		double last_server_info_request_check_time;
		rgl::string main_server_reply;

#if defined WARBAND
		void connect_to_server(const unsigned __int64 &vac_id, const rgl::string &ip, const int &port, const rgl::string &password);
#endif
	};
}
