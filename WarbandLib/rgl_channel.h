#pragma once

#include "rgl.h"
#include "rgl_quaternion.h"
#include "rgl_skeleton_anim.h"
#include "rgl_vector4.h"

namespace rgl
{
	struct channel
	{
		float blend_progress;
		float u1;
		unsigned __int64 enforced_bones;
		unsigned __int64 rotated_bones;
		float parameter;
		bool u2;
		bool start_instantly;
		float total_progress;
		float cycle_progress;
		float blend_threshold;
		float unblend_factor;
		float blend_factor;
		int u3;
		float u4;
		int u5;
		int u6;
		int u7;
		int u8;
		skeleton_anim *skeleton_anim;
		int u9;
		int u10;
		quaternion *bone_rotations;
		vector4 displace;
		int u11;
		int u12;
		int u13;
		int u14;
		int u15;
		int u16;
		int u17;
		int u18;
		skeleton_anim_spec *anim_spec;
		int u19;
		float duration;
		int u20;
		int u21;
		int u22;
		bool cyclic;
		int u23;
		float u33;
		float u34;
		float parallel_factors[2];
		int parallel_time_point_offsets[2];
		int u24;
	};
}
