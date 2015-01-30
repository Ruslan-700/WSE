#pragma once

#include "d3dx9.h"
#include "rgl_hlsl_technique.h"
#include "rgl_resource.h"
#include "rgl_string.h"

namespace rgl
{
	enum shader_requirement
	{
		shrf_lo_quality  = 0x1000, // requires config setting use_pixel_shaders and video card PS 1.1 capability
		shrf_mid_quality = 0x2000, // requires config setting shader_quality > 0
		shrf_hi_quality  = 0x4000, // requires config setting shader_quality > 1 and some additional video card PS 2.0a/b capabilities
	};

	enum shader_flags
	{
		shf_specular_enable   = 0x20,
		shf_static_lighting   = 0x80,
		shf_water_reflections = 0x100,
		shf_preshaded         = 0x1000,
		shf_uses_instancing   = 0x2000,
		shf_biased            = 0x8000,
		shf_always_fail       = 0x10000,
		shf_special           = 0x20000,
		shf_uses_pixel_shader = 0x10000000,
		shf_uses_hlsl         = 0x20000000,
		shf_uses_normal_map   = 0x40000000,
		shf_uses_skinning     = 0x80000000,
	};

	struct shader_stage
	{
		int material_texture_kind;
		unsigned int color_op;
		unsigned int alpha_op;
		unsigned int flags;
		int u1;
	};

	struct shader : resource
	{
		unsigned int flags;
		int u1;
		unsigned int requirement_flags;
		int num_alternatives;
		string alternatives[3];
		string technique_name;
		shader_stage stages[4];
		int num_stages;
		IDirect3DPixelShader9 *dx_pixel_shader;
		hlsl_technique *technique;
		bool created;
	};
}
