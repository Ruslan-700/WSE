#pragma once

#include "rgl_container_widget.h"
#include "rgl_simple_button_widget.h"
#include "rgl_widget.h"
#include "stl_vector.h"

namespace rgl
{
	struct combo_label_widget : container_widget
	{
		simple_button_widget *left_arrow_button_widget;
		simple_button_widget *right_arrow_button_widget;
		widget *label_widget;
		stl::vector<string> texts;
		bool first_to_last;
	};
}
