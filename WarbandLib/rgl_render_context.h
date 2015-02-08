#pragma once

#include "rgl_camera.h"
#include "rgl_light.h"
#include "rgl_postfx_node.h"
#include "rgl_render_mesh.h"
#include "rgl_render_params.h"
#include "rgl_scene.h"
#include "rgl_texture.h"
#include "rgl_vector4.h"
#include "stl_vector.h"

namespace rgl
{
	struct scene;
	
	enum render_context_flags
	{
		rcf_ignore_render_order = 0x1,
		rcf_clear_render_target = 0x100,
		rcf_clear_depth_buffer  = 0x200,
		rcf_add_at_end          = 0x1000,
		rcf_fixed_water_level   = 0x4000,
	};

	struct render_context
	{
		texture *render_target;
		texture *screen_depth_render_target;
		texture *dof_hdr_aa_depth_stencil_render_target;
		texture *dof_hdr_aa_render_target;
		stl::vector<postfx_node *> postfx_nodes;
		texture *active_render_target;
		texture *water_reflection_render_target;
		texture *active_depth_stencil_render_target;
		D3DCUBEMAP_FACES render_target_cube_map_faces;
		bool render_postfx;
		render_context *water_reflection_render_context;
		float dof_range;
		float dof_focus;
		void *u1;
		int u2;//1166
		viewport def_viewport;
		viewport postfx_viewport;
		camera camera;
		bool lighting_enabled;
		bool clipping_enabled;
		bool force_shadow_shader;
		bool force_light_shadow_shader;
		int force_character_shadow_shader;
		bool invert_culling_handedness;
		bool invert_water_fog;
		bool is_final_render;
		vector4 clip_plane;
		vector4 ambient_color;
		vector4 ground_ambient_color;
		unsigned int flags;
		unsigned int background_color;
		stl::vector<light *> lights;
		bool added_to_renderer;
		bool use_alt_postfx_viewport;
		unsigned int visibility_flags;
		bool allow_add_to_renderer;
		bool degrade_entities;
		render_params *render_params;
		stl::vector<render_mesh *> render_meshes;
		scene *scene;
		stl::vector<mesh *> meshes;
		float flare_strength;
		render_context *source_render_context;
		matrix water_reflection_camera_projection_matrix;
		matrix water_reflection_camera_position;
	};
}
