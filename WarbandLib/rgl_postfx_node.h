#pragma once

#include "rgl_string.h"
#include "rgl_texture.h"

namespace rgl
{
	struct postfx_node
	{
		texture *output_render_target;
		int viewport_width;
		int viewport_height;
		string type;
		void *technique_handle;
		int passes[5];
		int num_frames_to_skip;
		int num_passes;
	};
}
