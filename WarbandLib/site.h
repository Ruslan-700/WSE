#pragma once

#include "rgl.h"
#include "basics.h"
#include "mission_object.h"
#include "simple_game_object.h"

namespace wb
{
	enum site_flags
	{
		sf_indoors = 0x1,
		sf_force_skybox = 0x2,
		sf_generate = 0x100,
		sf_randomize = 0x200,
		sf_auto_entry_points = 0x400,
		sf_no_horses = 0x800,
		sf_muddy_water = 0x1000,
	};

	struct site_visitor_entry
	{
		stl::deque<int> troop_nos;
		stl::deque<int> team_nos;
		stl::deque<int> group_nos;
		unsigned int dna;
	};
	
	struct site : simple_game_object
	{
		unsigned __int64 descriptor_checksum;
		unsigned __int64 sco_file_checksum;
		rgl::string id;
		rgl::string name;
		unsigned int flags;
		rgl::string mesh_name;
		rgl::string body_name;
		float min_x;
		float min_y;
		float max_x;
		float max_y;
		float water_level;
		rgl::string terrain_code;
		bool sco_file_loaded;
		int passage_menu_nos[16];
		int num_passage_menu_nos;
		int chest_troop_nos[16];
		int num_chest_troop_nos;
		stl::vector<mission_object> mission_objects;
		site_visitor_entry visitor_entries[128];
		rgl::io_binary_stream ground_paint_stream;
		rgl::io_binary_stream ai_mesh_stream;
		slot_manager slots;
		rgl::string outer_terrain_name;
	};
}
