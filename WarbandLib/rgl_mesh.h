#pragma once

#include "rgl_light.h"
#include "rgl_matrix.h"
#include "rgl_bounding_box.h"
#include "rgl_particle_system.h"
#include "rgl_resource.h"
#include "rgl_material.h"
#include "rgl_vector2.h"
#include "rgl_vector4.h"
#include "stl_vector.h"

namespace rgl
{
	enum mesh_flags
	{
		msf_gamma_corrected = 0x1000000,
	};

	struct mesh_face
	{
		short patch_node_nos[4];
	};

	struct mesh_patch_node
	{
		vector3 normal;
		vector3 tangent;
		vector3 binormal;
		vector3 morphed_normal;
		vector2 uv;
		short vertex_no;
		unsigned int color_1;
		unsigned int color_2;
		unsigned int specular_color;
	};

	struct mesh_vertex
	{
		vector3 position;
		vector3 morphed_position;
	};

	struct mesh_skin_data
	{
		float weights[4];
		char bone_nos[4];
	};

	struct mesh_vertex_group_pair
	{
		int vertex_no;
		float weight;
	};

	struct mesh_vertex_group
	{
		int group_no;
		stl::vector<mesh_vertex_group_pair> pairs;
	};

	struct mesh_vertex_key
	{
		stl::vector<vector3> positions;
		stl::vector<vector3> normals;
	};

	struct mesh_vertex_keys
	{
		mesh_vertex_key *keys;
		int *time_points;
		int num_keys;
	};

	enum mesh_buffer_type
	{
		mbt_vertex_buffer = 0,
		mbt_index_buffer  = 1,
	};

	struct mesh_buffer_update_rec
	{
		int vertex_declaration_no;
		int registered_vertex_declaration_no;
		//int vertex_buffer_index;
		int policy;
		//int prev_vertex_buffer_index;
		char registered_policy;
		//int num_uses;
		bool update;
		bool added;
		int u1;
		//int pos;
		int u2;
		int u3;
		int u4;
		int u5;
		int u6;
	};

	struct mesh : resource
	{
		mesh *base_mesh;
		string material_name;
		material *material;
		unsigned int flags;
		int billboard;
		bool is_collection_mesh;
		unsigned int material_color_1;
		unsigned int material_color_2;
		int frame_time;
		int cull_mode;
		bool use_motion_blur;
		matrix motion_blur_matrix;
		stl::vector<mesh_patch_node> patch_nodes;
		stl::vector<mesh_skin_data> skin_data;
		stl::vector<mesh_vertex> vertices;
		stl::vector<mesh_vertex_group> vertex_groups;
		stl::vector<mesh_face> faces;
		bool has_unique_faces;
		bool has_unique_vertex_data;
		int u4;
		stl::vector<particle_system *> particle_systems;
		light *light;
		float cur_vertex_anim_frame_time;
		float next_vertex_anim_frame_time;
		mesh_vertex_keys vertex_keys;
		matrix local_position;
		unsigned int visibility_flags;
		mesh_buffer_update_rec buffer_update_recs[2];
		matrix parent_position;
		matrix transform_matrix;
		int u1;
		unsigned __int64 render_order;
		matrix *bone_positions;
		int num_bones;
		bounding_box bounding_box;
		bool is_original;
		bool render_order_is_up_to_date;

		virtual ~mesh();
		mesh *create_copy() const;
		void create_unique_vertex_data();
		void set_light(rgl::light *light);
		void set_vertex_anim_frame_time(const float &time);
		void create_vertex_anim_morph(const float &time);
		mesh_vertex_keys *get_vertex_keys();
		mesh_vertex_keys *get_base_vertex_keys();
		void remove_particle_system(int particle_system_no = -1);
		void set_vertex_color(unsigned int color);
		void set_color(unsigned int color);
#if !defined WARBAND_DEDICATED
		void replace_material(rgl::material *new_material);
#endif
	};
}
