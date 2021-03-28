#pragma once

#include "rgl_body.h"
#include "rgl_hlsl_technique.h"
#include "rgl_material.h"
#include "rgl_meta_mesh.h"
#include "rgl_mesh.h"
#include "rgl_resource.h"
#include "rgl_shader.h"
#include "rgl_skeleton_anim.h"
#include "rgl_skeleton_model.h"
#include "rgl_texture.h"
#include "stl_list.h"
#include "stl_vector.h"

namespace rgl
{
	struct resource_manager
	{
		int num_textures;
		int num_materials;
		int num_shaders;
		stl::vector<string> resource_files;
		stl::vector<meta_mesh *> meta_meshes;
		stl::vector<mesh *> meshes;
		stl::vector<mesh *> hashed_meshes;
		int mesh_hashes[256];
		bool mesh_hashes_up_to_date;
		stl::vector<texture *> textures;
#if defined WARBAND
		stl::vector<texture *> render_target_textures;
#endif
		stl::vector<shader *> shaders;
		stl::vector<material *> render_target_materials;
		stl::vector<material *> materials;
		stl::vector<body *> bodies;
		stl::vector<skeleton_model *> skeleton_models;
		stl::vector<skeleton_anim *> skeleton_anims;
		stl::vector<hlsl_technique *> hlsl_techniques;
		stl::list<resource *> u1;
		stl::vector<resource *> u2;
		shader *def_shader;
		shader *def_shader_ffp;
		material *def_material;
		material *def_font_material;
		material *def_font_outline_material;
		material *def_font_background_material;
		material *def_matte_material;
		material *def_shadow_material;
		void *u3;
		void *u4;
		mesh *def_track_mesh;
		mesh *def_battle_track_mesh;
		mesh *physics_sphere_mesh;
		mesh *physics_capsule_mesh;
		void *u5;
		mesh *def_troop_label_banner_mesh;
		mesh *def_troop_label_banner_mesh_hl;
		bool u6; //1166
		string resource_path;
		string resource_path_copy;
		bool edit_mode;
		bool edit_mode_copy;
		int u8; //1168
		stl::vector<texture *> core_textures;
#if defined WARBAND
		stl::vector<resource *> ondemand_textures_1;
		stl::vector<resource *> ondemand_textures_2;
		stl::vector<resource *> ondemand_textures_3;
#elif defined WARBAND_DEDICATED //1166
		int u7[8]; //1166
#endif

#if !defined WARBAND_DEDICATED
		stl::vector<mesh *> get_meshes(const string &name);
		mesh *try_get_mesh(const string &name);
		material *try_get_material(const string &name);
		material *get_material(const string &name);
		material *get_default_material();
#endif
		void load_resource_file(const string &file, int modular, bool update);
	};
}
