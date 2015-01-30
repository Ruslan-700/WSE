#pragma once

#include "rgl.h"
#include "basics.h"

namespace wb
{
	struct info_page
	{
		rgl::string title;
		rgl::string text;
		rgl::string id;
		note_manager notes;
	};
}
