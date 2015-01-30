#pragma once

#include "rgl.h"

namespace wb
{
	struct presentation_container
	{
		stl::vector<rgl::widget *> overlays;
		int presentation_no;
		unsigned int flags;
		rgl::timer presentation_timer;
		float duration;
		rgl::container_widget *container_widget;
		int container_overlay_no;
		int u1;
	};
}
