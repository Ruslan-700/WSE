#pragma once

#include "rgl.h"

namespace wb
{
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
