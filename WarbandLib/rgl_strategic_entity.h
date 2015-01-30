#pragma once

#include "rgl_light.h"
#include "rgl_mesh_entity.h"
#include "rgl_particle_system.h"
#include "rgl_render_context.h"
#include "rgl_skeleton.h"
#include "rgl_texture.h"
#include "rgl_timer.h"
#include "stl_vector.h"

namespace rgl
{
	struct skeleton;

	struct strategic_entity : mesh_entity
	{
#if defined WARBAND
		float shadow_level_of_detail;
		float shadow_1_level_of_detail;
		float shadow_2_level_of_detail;
#endif
		entity *parent_entity;
		int u1;
		int u2;
#if defined WARBAND
		timer shadow_cast_timer;
		timer shadow_precise_timer;
		int shadow_precise_rendered;
		int shadow_cast_map_face_nos[64];
		int shadow_num_cast_map_face_nos;
		vector4 shadow_last_position;
		texture *shadow_texture;
		render_context *shadow_render_context;
		int u3;
		float shadow_transparency;
		int shadow_paired_entity_no;
		light *light;
		stl::vector<particle_system *> particle_systems;
		int num_renders;
#endif
		skeleton *skeleton;
		
#if defined WARBAND
		virtual void set_color(const unsigned int &color);
		virtual void set_alpha(const unsigned int &alpha);
		virtual void add_particle_system(particle_system *psys);
		virtual void clear_particle_systems();
		virtual void get_particle_system(int particle_system_no);
#endif
		virtual void add_body_part(body_part *bp, matrix pos, bool moveable);
#if defined WARBAND
		void set_light(rgl::light *light);
		void remove_particle_system(int particle_system_no = -1);
#endif
	};
}
