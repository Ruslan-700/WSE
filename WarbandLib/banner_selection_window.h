#pragma once

#include "rgl.h"
#include "game_window.h"
#include "presentation_container.h"

namespace wb
{
	struct banner_selection_window : game_window
	{
		rgl::popup_widget *popup_widget;
		presentation_container presentation_container;
	};
}
