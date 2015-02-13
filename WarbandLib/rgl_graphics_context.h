#pragma once

#include "d3dx9.h"
#include "rgl_matrix.h"
#include "stl_vector.h"

namespace rgl
{
	struct filtering_state_block
	{
		IDirect3DStateBlock9 *dx_state_block;
		int minification_filter;
		int magnification_filter;
		int mipmap_filter;
		int texture_address_mode;
		int max_anisotropy;
		int mipmap_lod_bias;
		bool u1;
	};

	struct culling_state_block
	{
		IDirect3DStateBlock9 *dx_state_block;
		int cull_mode;
		int fill_mode;
		bool enable_multisample_antialias;
		bool enable_scissor_testing;
	};

	struct alpha_blending_state_block
	{
		IDirect3DStateBlock9 *dx_state_block;
		bool enable_alpha_blend;
		int src_blend;
		int dest_blend;
		int blendop;
	};

	struct depth_buffering_state_block
	{
		IDirect3DStateBlock9 *dx_state_block;
		int z_func;
		bool disable_z;
		bool enable_z_write;
		int blendop;
	};

	struct graphics_context
	{
		bool in_scene;
		bool fog_enabled;
		bool lighting_enabled;
		bool lights_enabled[64];
		bool specular_enabled;
		bool normalize_normals_enabled;
		IDirect3DTexture9 *dx_texture[3];
		IDirect3DPixelShader9 *dx_pixel_shader;
		int texture_stage_color[3];
		int texture_stage_alpha[3];
		int texture_stage_transform_flags[3];
		int texture_stage_coordinate_set[3];
		int alpha_test_mode;
		int material_color_1;
		int material_color_2;
		int texture_address_mode[3];
		int u1[3];
		bool motion_blur_enabled;
		int u2;
		int culling_handedness;
		int cull_mode;
		void *dx_diffuse_map;
		int light_point_count;
		int light_indices[4];
		float material_power;
		vector4 texture_offset;
		float time_var;
		vector4 specular_color;
		IDirect3DDevice9 *dx_device;
		IDirect3D9 *dx_iface;
		bool support_check_done;
		bool supports_format;
		bool supports_dof;
		bool supports_hdr;
		bool u4;//1166
		bool is_nvidia;
		bool is_ati;
		D3DCAPS9 device_caps;
		int u5;//1166
		bool has_ati_adapter;
		int u6;//1166
		D3DFORMAT adapter_format;
		matrix view_matrix;
		int u3;
		/*
		filtering_state_block *default_sampler_states[16];
		filtering_state_block *postfx_sampler_states[5];
		alpha_blending_state_block *alpha_blending_sampler_states[5];
		depth_buffering_state_block *depth_buffering_sampler_states[4];
		culling_state_block *culling_sampler_states[5];
		depth_buffering_state_block *depth_buffering_sampler_state;
		alpha_blending_state_block *alpha_blending_sampler_state;
		culling_state_block *culling_sampler_state;
		stl::vector<D3DMULTISAMPLE_TYPE> dof_hdr_antialiasing_samples_available;
		stl::vector<D3DMULTISAMPLE_TYPE> antialiasing_samples_available;
		D3DXHANDLE hlsl_data_handles[50];
		*/
		int u7[146];//1166 temp
		bool apply_default_sampler_states;
	};
}
