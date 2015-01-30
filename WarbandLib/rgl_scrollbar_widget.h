#pragma once

#include "rgl_string.h"
#include "rgl_widget.h"

namespace rgl
{
	struct scrollbar_widget : widget
	{
		float u1;
		string slidebar_light_mesh_name;
		string slidebar_mid_mesh_name;
		string slidebar_dark_mesh_name;
		string scrollbar_light_mesh_name;
		string scrollbar_mid_mesh_name;
		string scrollbar_dark_mesh_name;
		int direction;
		float u2;
		float u3;
		int focused;
		float u4;
	};
}
