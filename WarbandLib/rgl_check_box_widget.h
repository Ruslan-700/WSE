#pragma once

#include "rgl_container_widget.h"
#include "rgl_simple_button_widget.h"
#include "rgl_widget.h"

namespace rgl
{
	struct check_box_widget : container_widget
	{
		simple_button_widget *box_button_widget;
		widget *label_widget;
		float box_scale;
	};
}
