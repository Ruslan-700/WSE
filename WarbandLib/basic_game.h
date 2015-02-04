#pragma once

#include "rgl.h"
#include "basics.h"

#define NUM_REGISTERS 128

namespace wb
{
	enum game_type
	{
		gt_single = 0,
		gt_multi_client = 1,
		gt_multi_server = 2,
		gt_multi_client_server = 3,
	};

	struct basic_game
	{
		rgl::string name;
		rgl::string website;
		rgl::string domain;
		rgl::string game_download_url;
		rgl::string module_download_url;
		rgl::string http_replies[8];
		int num_http_replies;
#if defined WARBAND_DEDICATED
		rgl::output_text_file_stream server_log_stream;
		rgl::output_text_file_stream ban_list_stream;
		rgl::string server_log_date;
#endif
		int screenshot_index;
		__int64 trigger_param_1;
		__int64 trigger_param_2;
		__int64 trigger_param_3;
		//1166 start
		__int64 trigger_param_4;
		__int64 trigger_param_5;
		__int64 trigger_param_6;
		__int64 trigger_param_7;
		__int64 trigger_param_8;
		//1166 end
		__int64 trigger_result;
		int fixed_point_multiplier;
		rgl::string result_string;
		slot_manager global_variables;
		__int64 registers[NUM_REGISTERS];
		rgl::matrix position_registers[NUM_REGISTERS];
		rgl::string string_registers[NUM_REGISTERS];
		bool game_initialized;
		bool reload_player_agent_items;
		rgl::strategic_entity *map_used_entities[8][8];
		rgl::strategic_entity *map_stored_entities[64];
		int map_dim[2];
		float map_min[2];
		float map_max[2];
		float map_size[2];
		rgl::scene *map_scene;
		rgl::manifold *map_manifold;
#if defined WARBAND
		stl::vector<rgl::mesh *> map_sea_meshes;
		stl::vector<rgl::mesh *> map_river_meshes;
		stl::vector<rgl::mesh *> map_foam_meshes;
		stl::vector<rgl::mesh *> map_wave_meshes;
#endif
		rgl::pool<rgl::ai_mesh_vertex> ai_mesh_vertex_pool;
		rgl::pool<rgl::ai_mesh_edge> ai_mesh_edge_pool;
		rgl::pool<rgl::ai_mesh_face> ai_mesh_face_pool;
		rgl::ai_mesh_container<rgl::ai_mesh_vertex> ai_mesh_vertices;
		rgl::ai_mesh_container<rgl::ai_mesh_edge> ai_mesh_edges;
		rgl::ai_mesh_container<rgl::ai_mesh_face> ai_mesh_faces;
		game_type game_type;
		unsigned int bit_mask_table[33];
		rgl::string tooltip_text;
		rgl::timer multiplayer_mission_start_timer;
		bool multiplayer_mission_starting;
		bool slow_motion_toggle;
		int conversation_speaker_troop_no;
		float camera_zoom;
		bool switching_module;
		rgl::string stored_address;
		rgl::string stored_password;
		rgl::string stored_profile_name;
		rgl::string u1;
		bool display_messages;
		bool display_tooltips;
		int u2;
		char ime_composition_string[100];
		int num_rep_rows;
		int num_rep_tables;
		stl::vector<rgl::string> commands;
		int cur_command;

		void parse_string(rgl::string &out_str, const rgl::string &in_str, const bool &url_encode = 0);
		bool is_safe_string(const rgl::string &str);
		bool is_multiplayer();
		bool is_client();
		bool is_server();
		bool is_dedicated_server();
		bool is_listen_server();
		void put_value(unsigned char *dstBuffer, int &dstCursor, const void *srcValue, int srcSize);
		void put_data(unsigned char *dstBuffer, int &dstCursor, const void *srcBuffer, int srcSize);
		void take_value(const unsigned char *srcBuffer, int &srcCursor, void *dstValue, int dstSize, int offsetSize);
		void take_data(const unsigned char *srcBuffer, int &srcCursor, void *dstBuffer, int dstSize, int offsetSize);
	};
}
