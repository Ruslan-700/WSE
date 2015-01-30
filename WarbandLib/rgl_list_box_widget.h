#pragma once

#include "rgl_container_widget.h"
#include "rgl_mesh.h"
#include "rgl_vector2.h"
#include "stl_vector.h"

namespace rgl
{
	struct list_box_widget : container_widget
	{
		bool u1;
		int u2;
		stl::vector<string> texts;
		mesh *meshes[10];
		vector2 combobox_child_button_size;
		float combobox_child_button_position_x;
		vector2 combobox_child_button_text_size;
		vector2 combobox_child_button_text_position;
		bool meshes_updated;
		unsigned int mesh_color;
	};
}
