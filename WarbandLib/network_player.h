#pragma once

#include "rgl.h"
#include "basics.h"
#include "face_generator.h"
#include "mbnet_peer.h"
#include "multiplayer_event.h"
#include "network_buffer.h"

namespace wb
{	
	enum network_player_status
	{
		nps_unassigned = 0,
		nps_info       = 1,
		nps_active     = 2,
	};

	enum network_player_privilege
	{
		npp_none = 0,
		npp_regular = 1,
		npp_private = 2,
		npp_admin = 3,
	};

	struct network_player_replication_row
	{
		int sent_packet_id;
		int received_packet_id;
		bool send;
		rgl::string description;
		int u1;
		int u2;
		int u3;
		int num_times_sent;
		int u4;
		unsigned __int64 app_time;
		int size_in_bytes;
		int u5;
	};

	struct network_player_rng
	{
		int value;
		int seed;
	};

	struct sent_replication_record
	{
		int table_no;
		int row_no;
		rgl::vector4 u1;
	};

	struct sent_packet_record
	{
		int packet_id;
		double send_time;
		int num_event_recs;
		int num_replication_recs;
		int event_recs[250];
		sent_replication_record replication_recs[2048];
	};

	struct network_player
	{
		network_player_status status;
		rgl::timer connect_delay_timer;
		__int64 connect_time;
		bool disconnecting;
		rgl::timer disconnect_timer;
		__int64 session_id;
		double auth_time;
		bool auth_rejected;
		mbnet_peer mbnet_peer;
		network_player_privilege privilege;
		rgl::string name;
		rgl::string name_lowercase;
		int skin_no;
		int banner_no;
		face_keys face_keys;
		int blood_enabled;
		int particles_enabled;
		stl::vector<void *> u1;
		stl::vector<network_player_replication_row> replication_tables[2];
		int highest_received_packet_id;
		int highest_confirmed_sent_packet_id;
		stl::vector<void *> u2;
		bool use_ping_to_calc_packet_send_period;
		bool ready_to_spawn;
		int agent_no;
		bool has_agent;
		int team_no;
		int troop_no;
		int gold;
		int ready;
		int entry_point_no;
		bool is_muted;
		bool is_muted_globally;
		int spawn_items[9];
		int picked_up_items[9];
		slot_manager slots;
		stl::vector<multiplayer_event> events;
		stl::vector<multiplayer_event> one_time_events;
		double one_time_event_send_time;
		rgl::recycled_vector<sent_packet_record> sent_packet_records;
		stl::vector<int> unconfirmed_received_packet_ids;
		stl::vector<int> unconfirmed_sent_packet_ids;
		stl::vector<network_buffer> stalled_packets;
		stl::vector<int> stalled_packet_highest_received_packet_ids;
		double last_activity_time;
		double last_send_time;
		unsigned __int64 agent_update_frame_time;
		rgl::vector4 agent_update_position;
		rgl::vector2 agent_update_speed;
		float agent_update_movement_dirf;
		double agent_update_mission_time;
		short score;
		short kills;
		short deaths;
		short ping;
		int ping_total_time;
		int num_pings;
		bool pinging;
		double ping_send_time;
		double ping_time;
		float packet_send_period;
		int packet_avg_size;
		int packet_max_size;
		int traffic_limit;
		double next_packet_send_time;
		network_player_rng rng;
		bool busy_with_menus;
		int num_bytes_sent;
		int num_bytes_sent_per_sec;
		int num_packets_sent;
		int prev_num_packets_sent;
		int num_packet_sends_failed;
		float u3;
		float u4[200];
		float u5[200];
		unsigned __int64 steam_id;
		bool steam_data[2048];
		int num_steam_data;
		bool u6;

		bool is_active() const;
		unsigned int get_unique_id() const;
		void clear();
		void clear_items();
		void set_status(network_player_status new_status);
		void send_event(const multiplayer_event &evt);
		void send_one_time_event(const multiplayer_event &evt);
	};
}
