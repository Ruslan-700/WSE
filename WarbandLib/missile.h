#pragma once

#include "fmod.h"
#include "rgl.h"
#include "action.h"
#include "item.h"
#include "simple_game_object.h"

namespace wb
{
	enum missile_hit_type
	{
		mht_none = 0,
		mht_scene_remove = 1,
		mht_scene = 2,
		mht_agent = 3,
		mht_mission_object = 4,
		mht_shield = 5,
	};

	enum missile_screen_state
	{
		mss_not_present = 0,
		mss_flying      = 1,
		mss_hit         = 3,
	};

	struct missile : simple_game_object
	{
		int prev_path;
		int cur_path;
		bool is_client;
		missile_hit_type hit_type;
		int hit_mission_object_no;
		int hit_agent_no;
		int hit_bone_no;
		rgl::matrix hit_position;
		rgl::vector4 starting_client_position;
		rgl::vector4 starting_position;
		rgl::vector4 cur_position;
		rgl::vector4 prev_position;
		rgl::vector4 starting_direction;
		rgl::vector4 starting_movement;
		rgl::vector4 starting_error;
		rgl::vector4 path[500];
		rgl::strategic_entity *entity;
		float u1;
		int shooter_agent_no;
		float velocity;
		float damage;
		rgl::timer creation_timer;
		item shooting_item;
		item missile_item;
		bool hit_horse;
		float rotation_factor;
		missile_screen_state screen_state;
		FMOD_CHANNEL *fmod_channel;
		float sound_start_time;
		int invalidate_delay;
		attack attack_rec;
		
		rgl::vector4 get_velocity();
		rgl::vector4 get_direction();
	};
}
