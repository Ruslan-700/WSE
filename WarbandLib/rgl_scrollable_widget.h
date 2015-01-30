#pragma once

#include "rgl_camera.h"
#include "rgl_container_widget.h"
#include "rgl_render_context.h"
#include "rgl_scrollbar_widget.h"

namespace rgl
{
	struct scrollable_widget : container_widget
	{
		render_context render_context;
		camera camera;
		scrollbar_widget *scrollbar_widgets[2];
		int u1;
		int u2;
		container_widget *scroll_plate;
		float u3;
		float u4;
		float u5;
		float u6;
	};
}
