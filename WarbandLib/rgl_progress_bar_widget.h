#pragma once

#include "rgl_mesh.h"
#include "rgl_vector4.h"
#include "rgl_widget.h"

namespace rgl
{
	struct progress_bar_widget : widget
	{
		int u1;
		int u2;
		mesh *handle_mesh;
		bool redraw_handle;
		vector4 u3;
	};
}
