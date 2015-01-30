#pragma once

#include "rgl_button_widget.h"
#include "rgl_timer.h"

namespace rgl
{
	struct text_box_widget : button_widget
	{
		int u1;
		int u2;
		timer focus_timer;
		bool u3;
		bool focused;
		bool u4;
		int u5;
		bool is_numeric;
		int u6;
	};
}
