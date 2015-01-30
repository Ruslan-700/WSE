#pragma once

#include "d3dx9.h"
#include "rgl_material.h"
#include "rgl_texture.h"
#include "rgl_scene.h"
#include "rgl_render_context.h"

namespace rgl
{
	struct shadowmap_renderer
	{
		material *shadow_material_static;
		texture *shadow_render_target;
		IDirect3DSurface9 *shadow_surface;
		int shadow_texture_width;
		int shadow_texture_height;
		render_context shadow_render_context;
		scene *shadow_scene;
	};
}
