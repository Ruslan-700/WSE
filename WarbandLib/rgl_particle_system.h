#pragma once

#include "rgl_matrix.h"
#include "rgl_material.h"
#include "rgl_mesh.h"
#include "rgl_vector2.h"
#include "rgl_vector4.h"
#include "stl_vector.h"

namespace rgl
{
	struct mesh;

	struct particle_system_key
	{
		float time;
		float magnitude;
	};

	struct particle
	{
		matrix position;
		vector4 direction;
		int time;
		int u1;
		float rotation;
		float angular_speed;
		float size;
	};

	struct particle_system
	{
		mesh *render_mesh;
		material *material;
		mesh *particle_mesh;
		stl::vector<particle> particles;
		float turbulence_size;
		float turbulence_strength;
		float damping;
		particle_system_key alpha[2];
		particle_system_key red[2];
		particle_system_key green[2];
		particle_system_key blue[2];
		particle_system_key scale[2];
		float side_scale;
		float up_scale;
		vector2 uv[4];
		float life;
		vector4 u1;
		vector4 rotation;
		vector4 gravity;
		float gravity_strength;
		vector4 color;
		unsigned int flags;
		float water_level;
		int u2;
		matrix burst_position;
		matrix emit_box_position;
		vector4 emit_box_size;
		vector4 emit_velocity;
		float emit_randomness;
		float num_particles;
		int num_bursts_left;
		int burst_strength;
		float burst_num_particles;
		int particle_system_no;
		float radius;
		float angular_speed;
		float angular_damping;
		int timer_no;
	};
}
