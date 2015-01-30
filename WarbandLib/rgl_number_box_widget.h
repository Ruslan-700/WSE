#pragma once

#include "rgl_container_widget.h"
#include "rgl_simple_button_widget.h"
#include "rgl_text_box_widget.h"
#include "rgl_timer.h"

namespace rgl
{
	struct number_box_widget : container_widget
	{
		text_box_widget *text_box_widget;
		simple_button_widget *up_arrow_button_widget;
		simple_button_widget *down_arrow_button_widget;
		bool holding;
		timer hold_timer;
		float last_hold_check;
	};
}
