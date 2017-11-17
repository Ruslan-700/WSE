#pragma once

#include "rgl.h"
#include "basics.h"
#include "simple_game_object.h"

namespace wb
{
	enum mission_object_meta_type
	{
		mt_scene_prop    = 0,
		mt_entry_point   = 1,
		mt_scene_item    = 2,
		mt_baggage       = 3,
		mt_plant         = 4,
		mt_passage       = 5,
		mt_spawned_item  = 6,
		mt_missile_item  = 7,
		mt_death_item    = 8,
		mt_broken_shield = 9,
		mt_spawned_prop  = 10,
	};

	struct mission_object : simple_game_object
	{
		int source;
		int source_index;
		rgl::matrix position;
		rgl::matrix u1;
		rgl::matrix starting_position;
		rgl::matrix animate_target_position;
		rgl::matrix animate_starting_position;
		rgl::vector4 scale;
		int animate_state;
		int animate_duration;
		rgl::timer animate_timer;
		float animate_progress;
		float animate_total_rotation;
		float animate_cur_rotation;
		bool rotate;
		float total_rotation;
		float cur_rotation;
		rgl::timer enable_timer;
		int u2;
		mission_object_meta_type meta_type;
		int sub_kind_no;
		rgl::string id;
		int variation_no_1;
		int variation_no_2;
		int u3;
		int u4;
		rgl::strategic_entity *entity;
		float max_hit_points;
		float hit_points;
		int team_no;
		int u5;
		int u6;
		bool do_prune;
		rgl::timer prune_timer;
		slot_manager slots;
		int u7;
	};
}
