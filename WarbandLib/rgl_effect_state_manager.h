#pragma once

#include "d3dx9.h"

namespace rgl
{
	struct effect_state_manager : ID3DXEffectStateManager
	{
		IDirect3DDevice9 *dx_device;
		volatile LONG reference_count;
		IDirect3DVertexShader9 *dx_vertex_shader;
		IDirect3DPixelShader9 *dx_pixel_shader;
		IDirect3DBaseTexture9 *dx_textures[16];
	};
}
