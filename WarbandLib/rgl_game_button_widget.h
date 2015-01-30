#pragma once

#include "rgl_simple_button_widget.h"

namespace rgl
{
	struct game_button_widget : simple_button_widget
	{
		bool ignore_height;
	};
}
