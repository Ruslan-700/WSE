#pragma once

#include "stl_vector.h"
#include "Physics\Dynamics\World\hkpWorld.h"
#include "Physics\Dynamics\Entity\hkpRigidBody.h"
#include "Physics\Dynamics\Phantom\hkpSimpleShapePhantom.h"

namespace rgl
{
	enum collision_filter_info
	{
		cfi_agent               = 2,
		cfi_ai_agent            = 3,
		cfi_static_body         = 4,
		cfi_terrain             = 5,
		cfi_barrier             = 6,
		cfi_ai_limiter          = 7,
		cfi_mission_object      = 9,
		cfi_non_collidable      = 10,
		cfi_horse_agent         = 11,
		cfi_ai_horse_agent      = 12,
		cfi_dynamic_body        = 13,
		cfi_horse_agent_ragdoll = 14,
		cfi_barrier_3d          = 15,
	};

	struct ragdoll_physics_world
	{
		int u1;
		int activation_status;
		hkpPhysicsSystem *ragdoll;
		stl::vector<hkpRigidBody *> bone_rigid_bodies;
		stl::vector<hkpRigidBody *> horse_rigid_bodies;
		int u3;
	};

	struct physics_manager
	{
		stl::vector<ragdoll_physics_world> worlds;
		int num_active_worlds;
		int u1;
		hkThreadMemory *thread_memory;
		void *stack_buffer;
		hkpWorld *physics_world;
		stl::vector<hkpRigidBody *> u2;
		int u3;
		int u4;
		stl::vector<void *> u5;
		stl::vector<hkpEntity *> entities;
		stl::vector<hkpSimpleShapePhantom *> agent_phantoms;
		stl::vector<void *> u6;
		stl::vector<void *> u7;
	};
}
