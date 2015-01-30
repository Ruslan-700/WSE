#pragma once

#include "d3dx9.h"
#include "rgl_resource.h"

namespace rgl
{
	enum texture_flags
	{
		tf_no_mipmaps       = 0x1,
		tf_max_quality      = 0x2,
		tf_is_normal_map    = 0x4,
		tf_is_multilanguage = 0x8,
		tf_no_hdr           = 0x10,
		tf_hdr              = 0x20,
		tf_is_cubemap       = 0x40,
		tf_no_ondemand      = 0x80,
		tf_uv_mode_mask     = 0x300000,
		tf_num_frames_mask  = 0xF000000,
		tf_ondemand         = 0x10000000,
		tf_restore_ondemand = 0x20000000,
	};

	enum texture_type
	{
		rgl_tt_image              = 0,
		rgl_tt_render_target      = 1,
		rgl_tt_depth_stencil      = 2,
		rgl_tt_cube_render_target = 3,
	};

	struct texture : resource
	{
		bool valid;
		int u1;
		texture_type texture_type;
		unsigned int flags;
		int num_frames;
		int width;
		int height;
		int generate_mipmap_sublevels;
		D3DFORMAT render_target_format;
		D3DFORMAT depth_stencil_format;
		D3DMULTISAMPLE_TYPE multisample_type;
		IDirect3DTexture9 **dx_texture_frames;
		IDirect3DSurface9 *dx_depth_stencil_surface;
		IDirect3DSurface9 *dx_render_target_surface;
		IDirect3DCubeTexture9 *dx_cube_render_target_texture;
		int is_module_texture;
		int ondemand_state;
		IDirect3DTexture9 *dx_ondemand_texture;
		double load_time;
		int u2;
		int refresh_no;
	};
}
