#pragma once

#include "rgl_widget.h"

namespace rgl
{
	struct slider_widget : widget
	{
		int direction;
		int u1;
		int focused;
		int u2;
	};
}
