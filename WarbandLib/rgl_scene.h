#pragma once

#include "rgl_ai_mesh.h"
#include "rgl_body_part.h"
#include "rgl_camera.h"
#include "rgl_entity.h"
#include "rgl_light.h"
#include "rgl_manifold.h"
#include "rgl_matrix.h"
#include "rgl_particle_system.h"
#include "rgl_render_params.h"
#include "rgl_scene_grid.h"
#include "rgl_terrain.h"
#include "rgl_vector4.h"
#include "stl_list.h"
#include "stl_vector.h"

namespace rgl
{
	struct scene
	{
		scene_grid grid;
		stl::vector<body_part *> stored_bodies;
		terrain *terrain;
		vector4 ambient_color;
		vector4 ground_ambient_color;
		entity *skybox_entity;
		unsigned int screen_color;
		entity *low_detail_meshes_entity;
#if defined WARBAND
		int num_character_shadows_rendered;
#endif
		float character_shadow_render_distance;
		stl::vector<entity *> entities;
		stl::vector<entity *> queued_entities;
#if defined WARBAND
		stl::vector<entity *> degrading_entities;
#endif
		int num_entities_created;
		manifold *manifold;
		ai_mesh *ai_mesh;
#if defined (WARBAND)
		stl::vector<light *> lights;
#endif
		stl::vector<mesh *> particle_system_meshes;
#if defined (WARBAND)
		render_params render_params;
		vector4 global_light_color;
#endif
		vector4 camera_rotation;
		vector4 camera_position;
		stl::vector<particle_system *> particle_systems;
		int num_particle_systems;
		int timer_no;
		float character_level_of_detail;
		bool has_sun_flare;

		entity *get_entity(int no) const;
		void get_ground_data(const rgl::vector4 &position, float &height, rgl::quad **quad);
		void get_body_parts_in_path(const rgl::vector4 &start, const rgl::vector4 &end, unsigned int ignore_flags, rgl::body_part **body_parts, int &num_body_parts);
	};
}
