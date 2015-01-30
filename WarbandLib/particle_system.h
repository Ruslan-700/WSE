#pragma once

#include "rgl.h"

#define NUM_MAPPED_PARTICLE_SYSTEMS 11

namespace wb
{
	struct particle_system
	{
		rgl::string id;
		unsigned int flags;
		rgl::string mesh_name;
		rgl::mesh *mesh;
		float num_particles;
		int u1;
		int u2;
		float life;
		float damping;
		float gravity_strength;
		float turbulence_size;
		float turbulence_strength;
		rgl::particle_system_key alpha[2];
		rgl::particle_system_key red[2];
		rgl::particle_system_key green[2];
		rgl::particle_system_key blue[2];
		rgl::particle_system_key scale[2];
		rgl::vector4 emit_box_size;
		rgl::vector4 emit_velocity;
		float emit_randomness;
		float angular_speed;
		float angular_damping;

	};

	struct particle_system_manager
	{
		particle_system *particle_systems;
		int num_particle_systems;
		int mapped_particle_systems[NUM_MAPPED_PARTICLE_SYSTEMS];

		rgl::particle_system *create(const int &particle_system_no, const rgl::vector4 &position, const int &u1 = 0);
	};
}
