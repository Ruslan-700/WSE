#pragma once

#include "rgl_button_widget.h"

namespace rgl
{
	enum simple_button_widget_flags
	{
		sbwf_toggleable = 0x1,
		sbwf_clickonce = 0x2,
	};

	struct simple_button_widget : button_widget
	{
		bool is_down;
		unsigned int button_flags;
	};
}
