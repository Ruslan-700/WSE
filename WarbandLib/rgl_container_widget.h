#pragma once

#include "rgl_string.h"
#include "rgl_vector2.h"
#include "rgl_widget.h"
#include "stl_vector.h"

namespace rgl
{
	struct container_widget : widget
	{
		string container_name;
		stl::vector<widget *> child_widgets;
		float z_offset;

		virtual void clear_children();
		virtual void add_child(widget *widget);
		virtual void insert_child(widget *widget, const int &index);
		virtual void remove_child(widget *widget);
		virtual bool is_position_on_child(widget *, const vector2 &position);
	};
}
