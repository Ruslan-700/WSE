#pragma once

#include "rgl_widget.h"
#include "rgl_vector4.h"
#include "stl_vector.h"

namespace rgl
{
	struct hyperlink_text_widget : widget
	{
		stl::vector<vector4> link_position_upper_left_corners;
		stl::vector<vector4> link_position_lower_right_corners;
		stl::vector<int> object_types;
		stl::vector<int> object_nos;
		stl::vector<int> links;
		int hovered_link_no;
		int clicked_link_no;
	};
}
