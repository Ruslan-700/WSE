#pragma once

#include "rgl_render_context.h"
#include "rgl_scene.h"
#include "rgl_widget.h"

namespace rgl
{
	struct scene_widget : widget
	{
		scene *scene;
		render_context render_context;
		bool skip_next_frame;
	};
}
