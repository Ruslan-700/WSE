#pragma once

#include "rgl.h"

#define FACE_KEYS_MORPH_KEY_MASK (unsigned __int64)0x7
#define FACE_KEYS_VALUE_MASK (unsigned __int64)0x3F
#define FACE_KEYS_HAIR_SHIFT 0
#define FACE_KEYS_BEARD_SHIFT 6
#define FACE_KEYS_FACE_TEXTURE_SHIFT 12
#define FACE_KEYS_HAIR_TEXTURE_SHIFT 18
#define FACE_KEYS_HAIR_COLOR_SHIFT 24
#define FACE_KEYS_AGE_SHIFT 30
#define FACE_KEYS_SKIN_COLOR_SHIFT 36

namespace wb
{
	struct face_keys
	{
		unsigned __int64 keys[4];

		face_keys();		
		int get_hair() const;
		void set_hair(int value);
		int get_beard() const;
		void set_beard(int value);
		int get_face_texture() const;
		void set_face_texture(int value);
		int get_hair_texture() const;
		void set_hair_texture(int value);
		float get_hair_color() const;
		void set_hair_color(float value);
		float get_age() const;
		void set_age(float value);
		float get_skin_color() const;
		void set_skin_color(float value);
		float get_morph_key(int index) const;
		void set_morph_key(int index, float value);
		rgl::string to_string() const;
		void from_string(const rgl::string &facekeys);
	};

	struct face_texture_rec
	{
		rgl::string material_name;
		rgl::material *material;
		rgl::material *lod_material;
		unsigned int color;
		stl::vector<rgl::string> hair_textures;
		rgl::vector4 hair_colors[8];
		int num_hair_colors;
	};

	struct beard_rec
	{
		rgl::string mesh_name;
		int u1;
		rgl::meta_mesh *meta_mesh;
	};

	struct morph_rec
	{
		rgl::string mesh_name;
		int num_vertices[NUM_LODS];
		int *vertex_nos[NUM_LODS];
	};

	struct face_key_constraint_pair
	{
		float u1;
		int face_key_no;
	};

	struct face_key_constraint
	{
		int u1;
		face_key_constraint_pair pairs[63];
		int type;
		int u2;
	};

	struct face_key
	{
		rgl::string name;
		rgl::string id;
		int frame;
		float morph_min;
		float morph_max;
		unsigned int flags;
		rgl::vector4 *vertex_positions;
	};

	struct skin
	{
		rgl::string id;
		unsigned int flags;
#if defined WARBAND
		rgl::meta_mesh *body_meta_mesh;
		rgl::meta_mesh *left_hand_meta_mesh;
		rgl::meta_mesh *right_hand_meta_mesh;
		rgl::meta_mesh *left_calf_meta_mesh;
		rgl::meta_mesh *right_calf_meta_mesh;
		rgl::string head_mesh_name;
		rgl::meta_mesh *head_meta_mesh;
#endif
		int num_face_keys;
		face_key *face_keys;
		beard_rec *beard_recs;
#if defined WARBAND
		stl::vector<morph_rec> beard_morphs;
		stl::vector<morph_rec> hair_morphs;
		stl::vector<morph_rec> skin_morphs;
#endif
		stl::vector<face_key_constraint> constraints;
		int num_beard_meshes;
#if defined WARBAND
		rgl::meta_mesh *hair_meta_meshes[64];
#endif
		int num_hair_meshes;
		int num_face_textures;
		int num_hair_textures;
		int num_beard_textures;
		stl::vector<rgl::string> hair_textures;
		stl::vector<rgl::string> beard_textures;
		stl::vector<face_texture_rec> face_textures;
		int voice_sound_nos[8];
		rgl::skeleton_model *skeleton_model;
		float scale;
		int blood_particle_1_no;
		int blood_particle_2_no;
	};

	struct face_generator
	{
		int cur_beard;
		int cur_hair;
		int cur_skin_no;
		int cur_hair_texture;
		int u1;
		int cur_face_texture;
		stl::vector<skin> skins;
		int num_skins;
		float cur_morph_keys[63];
		float cur_age;
		float cur_hair_color;
		rgl::meta_mesh *cached_meta_meshes[32][3];
		face_keys cached_keys_1[32];
		face_keys cached_keys_2[32];
		int cached_dnas[16];
	};
}
