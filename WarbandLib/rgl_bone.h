#pragma once

#include "rgl_light.h"
#include "rgl_matrix.h"
#include "rgl_mesh.h"
#include "rgl_meta_mesh.h"
#include "rgl_particle_system.h"
#include "rgl_quaternion.h"
#include "rgl_skeleton_anim.h"
#include "rgl_skeleton_model.h"
#include "rgl_vector4.h"
#include "stl_vector.h"
#include "Physics\Dynamics\Entity\hkpRigidBody.h"

namespace rgl
{
	struct bone
	{
		matrix starting_position;
		matrix local_position;
		matrix position;
		quaternion local_rotation;
		vector4 u1;
		quaternion parent_rotation;
		quaternion parent_root_rotation;
		vector4 rotation_vec;
		orientation external_rotation;
		int has_external_rotation;
		int ipo_keys[2][2];
		rgl::skeleton_anim *animations[2];
		rgl::quaternion start_rotations[2][2];
		rgl::quaternion end_rotations[2][2];
		stl::vector<mesh *> meshes;
		stl::vector<meta_mesh *> meta_meshes;
		int parent_bone_no;
		int num_attached_bones;
		int *attached_bones;
		skeleton_model_bone *model_bone;
		float blend_factor;
		float blend_strength;
		hkpRigidBody *havok_body;
		
#if defined WARBAND
		virtual void add_particle_system(particle_system *particle_system);
		virtual void set_light(light *light);
		virtual void find_particle_system(int &result, bool &on_meta_mesh, int particle_system_no);
#endif
		void remove_meta_mesh(meta_mesh *meta_mesh);
		void remove_particle_system(int particle_system_no);
	};
}
