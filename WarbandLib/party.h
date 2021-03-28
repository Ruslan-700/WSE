#pragma once

#include "rgl.h"
#include "basics.h"
#include "game_object.h"

#define MB_NUM_DNA_PARTY_STACKS 32

namespace wb
{
	struct party_stack
	{
		int num_troops;
		int troop_no;
		unsigned int flags;
		int num_spawned;
		int num_wounded;
		int num_upgradeable;
		float experience;
		int u1;
	};

	struct party : game_object
	{
		rgl::string name;
		bool renamed;
		rgl::string id;
		rgl::vector2 position;
		float u1[7];
		rgl::string extra_text;
		float radius;
		float label_transparency;
		int label_visible;
		int faction_no;
		int personality;
		int ignore_with_player_party;
		float bandit_attraction;
		float morale;
		float hunger;
		int u2;
		int event_no;
		int party_template_no;
		int menu_no;
		int banner_map_icon_no;
		int extra_icon_no;
		float extra_icon_up_down_distance;
		float extra_icon_up_down_frequency;
		float extra_icon_rotate_frequency;
		float extra_icon_fade_frequency;
		int u3;
		unsigned __int64 flags;
		stl::vector<party_stack> stacks;
		int num_stacks;
		int marshall;
		float speed;
		float speed_multiplier;
		bool update_speed;
		bool update_flags;
		stl::vector<int> attached_party_nos;
		int attached_to_party_no;
		bool attached;
		rgl::timer behavior_update_timer;
		float behavior_update_time;
		int ai_default_behavior;
		int ai_default_behavior_object;
		rgl::vector2 initial_position;
		float patrol_radius;
		bool u4;
		int u5;
		rgl::timer u6;
		rgl::timer walk_sound_timer;
		int ai_current_behavior;
		int ai_current_behavior_object;
		int u7;
		rgl::vector2 ai_last_known_target_position;
		rgl::vector2 position_random_factor;
		float target_update_time;
		rgl::timer target_update_timer;
		rgl::timer movement_update_timer;
		float initiative;
		float helpfulness;
		rgl::vector2 u8;
		rgl::vector2 destination_position;
		int path_scene_manifold_face_nos[1024];
		rgl::vector4 path_positions[1024];
		int path_begin;
		int path_end;
		rgl::quad *cur_quad;
		int cur_terrain_type;
		int u9[7];
		int move_mode;
		rgl::vector2 move_target_position;
		rgl::quad *move_target_quad;
		int move_target_party_no;
		rgl::timer hide_timer;
		rgl::strategic_entity *entity;
		float track_interval;
		rgl::vector4 track_position;
		slot_manager slots;
#if defined WARBAND
		rgl::mesh *map_legend_mesh;
		rgl::mesh *map_icon_mesh;
		rgl::mesh *map_icon_banner_mesh;
		rgl::mesh *map_icon_additional_mesh;
#endif
		stl::vector<int> particle_system_nos;
		bool create_particle_systems;
		note_manager notes;
		rgl::timer ignore_player_timer;
		int u10;
	};
}
