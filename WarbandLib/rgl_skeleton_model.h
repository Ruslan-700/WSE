#pragma once

#include "rgl_matrix.h"
#include "rgl_resource.h"
#include "rgl_vector4.h"
#include "stl_vector.h"

namespace rgl
{
	enum skeleton_model_body_flags
	{
		smbf_for_ragdoll_only   = 0x1,
		smbf_for_collision_only = 0x2,
	};

	struct skeleton_model_body
	{
		unsigned int flags;
		vector4 pos;
		vector4 pos2;
		float radius;
		short u1;
		int u2;
	};

	struct skeleton_model_bone
	{
		int u1;
		int attachment_bone_no;
		unsigned int flags;
		matrix position;
		matrix parent_position;
		string name;
		float mass;
		float cone_min;
		float cone_max;
		vector4 rot;
		vector4 socket_dir;
		float plane_min;
		float plane_max;
		vector4 plane_dir;
		float twist_limit_min;
		float twist_limit_max;
		stl::vector<skeleton_model_body> bodies;
		stl::vector<skeleton_model_body *> ragdoll_bodies;
		stl::vector<skeleton_model_body *> collision_bodies;
	};

	struct skeleton_model : resource
	{
		int num_bones;
		int u1;
		skeleton_model_bone *bones;
		vector4 *bone_positions;
		int num_bone_positions;
		unsigned int lower_body_bones;
		unsigned int right_side_bones;
		unsigned int all_bones;
	};
}
