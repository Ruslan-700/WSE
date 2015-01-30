#pragma once

#include "rgl.h"
#include "multiplayer_event.h"

namespace wb
{
	struct network_server
	{
		int upload_limit;
		//int u1;
		//__int64 packet_total_size_in_bits;
		//rgl::static_deque<int> packet_sizes_in_bits;
		//rgl::static_deque<double> packet_send_times;
		int port;
		int steam_port;
		double last_announcement_time;
		int cur_announcement_player_no;
#if defined WARBAND_DEDICATED
		rgl::string log_folder;
		rgl::string ban_list_file;
		stl::vector<int> permanently_banned_unique_ids;
		float ban_list_next_check_time;
#endif
		stl::vector<int> temporarily_banned_unique_ids;
		stl::vector<float> ban_durations;
		int auth_keys[10];
		int cur_auth_key_no;
		
		void broadcast_event(const multiplayer_event &evt);
		void broadcast_event(const multiplayer_event &evt, int except);
		bool is_team_chat_visible(const int &sender_no, const int &receiver_no) const;
		bool is_chat_visible(const int &sender_no, const int &receiver_no) const;
	};
}
