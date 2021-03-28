#pragma once

#include "rgl_matrix.h"
#include "rgl_mesh.h"
#include "rgl_bounding_box.h"
#include "rgl_string.h"

#define NUM_LODS 5

namespace rgl
{
	struct meta_mesh_lod
	{
		stl::vector<mesh *> meshes;

		~meta_mesh_lod();
		void set_mesh_vertex_anim_frame_time(const float &time);
		void set_base_mesh_vertex_anim_frame_time(const float &time);
		void create_vertex_anim_morph(const float &time);
		void add_mesh(mesh *mesh);
	};

	struct meta_mesh
	{
		float deformStartFrame;
		float deformEndFrame;
		float deformDuration;
		int deformMode;
		float deformStartTime;
		bounding_box bounding_box;
		meta_mesh_lod lods[NUM_LODS];
		int num_lods;
		int cur_lod;
		matrix position;
		unsigned int visibility_flags;
		string mesh_prefix;
		
		meta_mesh();
		meta_mesh *create_copy() const;
		void set_mesh_vertex_anim_frame_time(const float &time);
		void set_base_mesh_vertex_anim_frame_time(const float &time);
		void create_vertex_anim_morph(const float &time);
		float get_mesh_vertex_anim_frame_time();
		void start_deform_animation(int mode, float startFrame, float endFrame, float duration);
		float get_current_deform_animation_progress();
		void deform_move();
		void remove_particle_system(int particle_system_no = -1);
		void set_vertex_color(unsigned int color);
		void set_visibility_flags(unsigned int color);
		void add_mesh(int lodNo, mesh *mesh);
	};
}
