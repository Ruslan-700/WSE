#pragma once

#include "rgl.h"

namespace wb
{
	enum action_flags
	{
		acf_synch_with_horse = 0x1,
		acf_align_with_ground = 0x2,
		acf_enforce_lowerbody = 0x100,
		acf_enforce_rightside = 0x200,
		acf_enforce_all = 0x400,
		acf_lock_rotation = 0x800, //WSE
		acf_parallels_for_look_slope = 0x1000,
		acf_lock_camera = 0x2000,
		acf_displace_position = 0x4000,
		acf_ignore_slope = 0x8000,
		acf_thrust = 0x10000,
		acf_right_cut = 0x20000,
		acf_left_cut = 0x40000,
		acf_overswing = 0x80000,
		acf_rot_vertical_bow = 0x100000,
		acf_rot_vertical_sword = 0x200000,
		acf_rot_vertical_mask = 0x300000,
	};

	struct attack
	{
		int direction;
		int hit_agent;
		int hit_mission_object;
		int u1;
		float yell_time;
		float hold_time;
		int crushtrough;
		int u2;
		rgl::matrix prev_sword_frame;
		rgl::matrix cur_sword_frame;
	};

	struct animation
	{
		int u1;
		unsigned int flags;
		float sound_times[4];
		rgl::skeleton_anim_spec anim_spec;
		int bone_frames[64];
		rgl::vector4 displace;
		float ragdoll_delay;
	};

	struct action
	{
		rgl::string id;
		animation *animations;
		int num_animations;
		unsigned int flags;
		rgl::string u1;
		int length;
		unsigned int master_flags;
	};

	struct action_set
	{
		action *actions;
		int num_actions;
	};
}
