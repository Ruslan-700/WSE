#pragma once

#include "rgl_string.h"
#include "rgl_widget.h"

namespace rgl
{
	struct scrollbar_widget : widget
	{
		float handle_thickness;
		string slidebar_light_mesh_name;
		string slidebar_mid_mesh_name;
		string slidebar_dark_mesh_name;
		string scrollbar_light_mesh_name;
		string scrollbar_mid_mesh_name;
		string scrollbar_dark_mesh_name;
		int direction;
		float range;
		float handle_size;
		int focused;
		float focus_point;
	};
}
