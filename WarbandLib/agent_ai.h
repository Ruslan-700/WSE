#pragma once

#include "rgl.h"

namespace wb
{
	enum agent_ai_flags
	{
		aif_start_alarmed = 0x10,
	};

	enum agent_ai_look_target_flags
	{
		ailtf_look_at_position = 0x1,
		ailtf_look_at_agent    = 0x2,
		ailtf_aim_at_agent     = 0x20,
	};

	enum agent_ai_simple_behavior
	{
		aisb_hold               = 0,
		aisb_go_to_pos          = 1,
		aisb_mount              = 2,
		aisb_dismount           = 3,
		aisb_melee              = 4,
		aisb_ranged             = 5,
		aisb_ranged_horseback   = 6,
		aisb_charge_horseback   = 7,
		aisb_maneuver_horseback = 8,
		aisb_flock              = 9,
		aisb_race               = 10,
		aisb_patrol             = 11,
		aisb_no_enemies         = 12,
		aisb_horse_hold         = 13,
		aisb_horse_run_away     = 14,
	};

	struct agent_control
	{
		float look_dirf;
		rgl::vector4 look_dir;
		float rotation;
		float rotation_time;
		unsigned int actions_1;
		unsigned int actions_2;
		bool camera_mode;
	};

	struct agent_ai_queued_control
	{
		agent_control control;
		unsigned __int64 frame_time;
	};

	struct agent_ai_tracked_agent
	{
		rgl::vector4 diff_dir;
		bool diff_computed;
		int agent_no;
		float dist_sqr;
		rgl::vector4 diff;
		rgl::body_part *body_part;
	};

	struct agent_ai_tracker
	{
		agent_ai_tracked_agent tracked_agents[16];
		int num_tracked_agents;
		float u1;
	};

	struct agent_ai_path_point
	{
		rgl::vector3 position;
		rgl::ai_mesh_edge *edge;
	};

	struct agent_ai
	{
		agent_ai_flags flags;
		rgl::vector4 target_position;
		rgl::vector4 target_direction;
		rgl::ai_mesh_face *target_ai_mesh_face;
		rgl::ai_mesh_face *u1;
		int compute_path;
		float u2;
		float u3;
		rgl::vector4 look_dir;
		float u4;
		int u5;
		//int u6;
		//int u7;
		rgl::timer weapon_usage_order_change_timer;
		rgl::timer behavior_update_timer;
		float behavior_update_time;
		bool no_time_to_reload;
		int behavior_flags;
		int behavior;
		int behavior_target_agent_no;
		rgl::vector4 behavior_target_position;
		int ranged_target_obstruction;
		int melee_movement_status;
		int prev_melee_movement_status;
		float movement_update_time;
		rgl::timer movement_update_timer;
		rgl::timer actions_update_timer;
		float actions_update_time;
		int u8;
		rgl::timer avoid_ally_timer;
		rgl::timer u9;
		bool u10;
		float u11;
		int move_target_agent_no;
		int look_target_agent_no;
		int u12;
		rgl::ai_mesh_face *path_ai_mesh_faces[1024];
		wb::agent_ai_path_point path_points[1024];
		int path_valid;
		int path_begin;
		int path_end;
		bool u13;
		int path_length;
		int move_flags;
		int look_flags;
		rgl::vector2 u14;
		rgl::vector2 u15;
		rgl::vector4 look_target_position;
		float ideal_horse_speed;
		float obstruction;
		rgl::vector2 u16;
		rgl::timer u17;
		rgl::timer u18;
		int u19;
		int u20;
		int combat_state;
		int u21;
		int ranged_target_status;
		agent_control control;
		unsigned int combat_control_flags;
		stl::deque<agent_ai_queued_control> queued_actions;
		agent_control action_control;
		agent_ai_tracker tracker;
		rgl::matrix scripted_destination;
		int scripted_mode;
		int u22;
		int always_attack_in_melee;
		int damager_agent_no;
		int look_position_randomness;
		float speed_limit;
		float aggressiveness;
		bool u23;
	};
}
