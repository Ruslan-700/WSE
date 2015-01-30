#pragma once

#include "rgl_resource.h"
#include "rgl_shader.h"
#include "rgl_string.h"
#include "rgl_texture.h"
#include "rgl_vector4.h"

namespace rgl
{
	enum material_flags
	{
		mf_no_fog               = 0x1,
		mf_no_lighting          = 0x2,
		mf_no_depth_effects     = 0x4,
		mf_no_depth_write       = 0x8,
		mf_no_depth_test        = 0x10,
		mf_uniform_shading      = 0x40,
		mf_alpha_blend_modulate = 0x100,
		mf_alpha_blend_add      = 0x200,
		mf_alpha_blend_multiply = 0x400,
		mf_alpha_blend_factor   = 0x700,
		mf_alpha_test_1         = 0x1000,
		mf_alpha_test_128       = 0x2000,
		mf_alpha_test_255       = 0x3000,
		mf_render_first         = 0x10000,
		mf_origin_at_camera     = 0x20000,
		mf_lod                  = 0x40000,
		mf_lod_if_low_poly      = 0x80000,
		mf_render_order_plus_1  = 0x1000000,
		mf_render_order_plus_2  = 0x2000000,
		mf_render_order_plus_3  = 0x3000000,
		mf_render_order_plus_4  = 0x4000000,
		mf_render_order_plus_5  = 0x5000000,
		mf_render_order_plus_6  = 0x6000000,
		mf_render_order_plus_7  = 0x7000000,
		mf_render_order_minus_8 = 0x8000000,
		mf_render_order_minus_7 = 0x9000000,
		mf_render_order_minus_6 = 0xA000000,
		mf_render_order_minus_5 = 0xB000000,
		mf_render_order_minus_4 = 0xC000000,
		mf_render_order_minus_3 = 0xD000000,
		mf_render_order_minus_2 = 0xE000000,
		mf_render_order_minus_1 = 0xF000000,
	};

	enum material_texture_kind
	{
		mtk_diffuse_1   = 0,
		mtk_diffuse_2   = 1,
		mtk_bump        = 2,
		mtk_environment = 3,
		mtk_specular    = 4,
	};

	struct material : resource
	{
		int u1;
		unsigned int flags;
		string shader_name;
		vector4 specular_color;
		float specular_coefficient;
#if defined WARBAND
		shader *shader;
		texture *textures[5];
		int tableau_no;
		int tableau_param;
		int num_uses;
		void *tableau_func;
#endif
	};
}
