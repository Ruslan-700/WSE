#pragma once

#include "rgl_button_widget.h"

namespace rgl
{
	struct simple_button_widget : button_widget
	{
		bool is_down;
		unsigned int button_flags;
	};
}
