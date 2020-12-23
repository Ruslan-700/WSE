#pragma once

#include "rgl.h"

#define MB_NUM_TERRAIN_KEYS 6

#define tk_seed_mask 0x7FFFFFFF
#define tk_seed_shift 0
#define tk1_deep_water_mask 0x80000000
#define tk1_deep_water_shift 31
#define tk3_size_x_mask 0x3FF
#define tk3_size_x_shift 0
#define tk3_size_y_mask 0xFFC00
#define tk3_size_y_shift 10
#define tk3_place_river_mask 0x80000000
#define tk3_place_river_shift 31
#define tk4_valley_mask 0x7F
#define tk4_valley_shift 0
#define tk4_hill_height_mask 0x3F80
#define tk4_hill_height_shift 7
#define tk4_ruggedness_mask 0x1FC000
#define tk4_ruggedness_shift 14
#define tk4_vegetation_mask 0xFE00000
#define tk4_vegetation_shift 21
#define tk4_region_type_mask 0xF0000000
#define tk4_region_type_shift 28
#define tk5_region_detail_mask 0x3
#define tk5_region_detail_shift 0
#define tk5_disable_grass_mask 0xC
#define tk5_disable_grass_shift 2

namespace wb
{
	struct terrain_vertex
	{
		rgl::vector4 position;
		int mesh_face_corner_no;
		int manifold_vertex_no;
		float slope;
		unsigned int terrain_type_flags;
		rgl::vector4 normal;
		rgl::vector2 uv;
		rgl::vector2 river_direction;
		float illumination;
		int river_curve_no;
		float layer_intensities[15];
		float altitude;
		float altitude_increase;
		unsigned int color;
		float transparency;
	};

	struct terrain_face
	{
		int orientation;
		int ter_type[2];
		rgl::vector4 normals[2];
		int u1[2];
		float u2;
		float layer_intensities[15];
	};

	struct fauna_rec
	{
		int flora_kind_no;
		float radius;
		float treshold;
		rgl::vector4 randomness;
		float density;
	};

	struct flora_rec
	{
		int id;
		int terrain_face[2];
		int face_orientation;
		int u1;
		rgl::matrix position;
		rgl::vector4 scale;
		int instance_no;
		int u2;
		rgl::mesh *meshes[4];
		int num_vertices_before[4];
		int num_vertices_after[4];
		int num_meshes;
		bool entity_created;
	};

	struct flora_set
	{
		int num_placed_flora;
		int max_fauna_to_place;
		flora_rec *flora_recs;
		int fauna_table_size;
		fauna_rec fauna_table[32];
	};

	struct ground_texture_layer
	{
		rgl::string u1;
		int ground_spec_no;
		unsigned int flags;
		int terrain_type;
		rgl::material *material;
		rgl::material *multitex_material;
		float uv_scale;
	};
	
	struct terrain_generator
	{
		unsigned int terrain_code[6];
		ground_texture_layer ground_texture_layers[15];
		float size[2];
		int num_faces[2];
		int u1;
		int num_river_passes;
		terrain_vertex terrain_vertices[251][251];
		terrain_face terrain_faces[250][250];
		rgl::scene *scene;
		float region_detail;
		flora_set rock_flora_set;
		flora_set tree_flora_set;
		flora_set grass_flora_set;
		flora_set speedtree_flora_set;
		int barrenness;
		rgl::vector4 offset;
		rgl::vector4 center_light_direction;
		rgl::vector4 center_light_color;
		rgl::vector4 ambient_color;
		rgl::vector4 ground_ambient_color;
		rgl::vector4 diffuse_color;
		float noise_frequency;
		rgl::vector4 noise_position_randomness;
		rgl::vector4 noise_position_color_randomness;
		int terrain_block_size;
		bool u2;
		bool place_river;
#if defined WARBAND
		rgl::strategic_entity *grass_entities[48][48];
		int grass_entities_populated[48][48];
		rgl::bounding_box grass_bounding_boxes[48][48];
#endif
		int u3;
#if defined WARBAND
		int num_grass_entities[2];
		int num_terrain_entities[2];
#endif
		rgl::strategic_entity *terrain_entities[5][5];
		rgl::strategic_entity *low_detail_meshes_entity;
#if defined WARBAND
		rgl::mesh *shadow_meshes[5][5];
		rgl::mesh *terrain_meshes[15][5][5];
#endif
		bool u4[15][5][5][5];
#if defined WARBAND
		rgl::string outer_terrain_name;
		rgl::strategic_entity *outer_terrain_entities[9];
		stl::vector<rgl::mesh *> outer_terrain_meshes;
#endif
		rgl::manifold *manifold;
		stl::vector<rgl::vector4> terrain_vertices_for_havok;
		stl::vector<int> terrain_faces_for_havok;
	};
}
