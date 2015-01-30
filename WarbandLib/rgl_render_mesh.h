#pragma once

#include "rgl_material.h"
#include "rgl_matrix.h"
#include "rgl_mesh.h"
#include "stl_vector.h"

namespace rgl
{
	struct render_mesh_buffer_update_rec
	{
		int vdt_no;
		int index;
		int policy;
		int pos;
	};

	struct render_mesh
	{
		mesh *base_mesh;
		material *material;
		unsigned int flags;
		int billboard;
		bool is_collection_mesh;
		unsigned int material_color_1;
		unsigned int material_color_2;
		float radius;
		float frame_time;
		int cull_mode;
		bool use_motion_blur;
		unsigned int visibility_flags;
		render_mesh_buffer_update_rec buffer_update_recs[2];
		matrix parent_position;
		matrix transform_matrix;
		matrix motion_blur_matrix;
		int num_bones;
		matrix *bone_positions;
		matrix bone_positions_buffer[32];
		int num_vertices;
		int num_tris_vertices;
		bool has_unique_faces;
		bool has_unique_vertex_data;
		float cur_vertex_animation_frame_time;
		render_mesh *source_render_mesh;
		mesh *mesh;
		stl::vector<float *> instances;
	};
}