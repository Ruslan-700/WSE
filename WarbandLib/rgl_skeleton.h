#pragma once

#include "rgl.h"
#include "rgl_channel.h"
#include "rgl_matrix.h"
#include "rgl_mesh.h"
#include "rgl_meta_mesh.h"
#include "rgl_quaternion.h"
#include "rgl_skeleton_model.h"
#include "rgl_vector4.h"
#include "stl_vector.h"

namespace rgl
{
	struct skeleton
	{
		int num_bones;
		bone *bones;
		skeleton_model *model;
		vector4 next_offset;
		vector4 prev_offset;
		int ipo_key;
		stl::vector<mesh *> meshes;
		stl::vector<meta_mesh *> meta_meshes;
		channel *channels;
		int num_channels;
		rgl::quaternion *u1;
		int u2;
		bool bones_updated;
		float next_update_time;
		float prev_update_time;
		bool skinning_updated;
		int u3;
		int timer_no;
		int is_ragdoll;
		bool u4;
		vector4 cur_offset;

		const matrix &get_bone_position(const int &bone_no);
		void remove_meta_mesh(meta_mesh *meta_mesh);
		void update_bone_positions();
	};
}
