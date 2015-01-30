#pragma once

#include "rgl.h"

namespace wb
{
	struct skybox
	{
		rgl::string id;
		unsigned int flags;
		float sun_heading;
		float sun_altitude;
		float flare_strength;
		rgl::string post_effect;
		rgl::vector4 sun_color;
		rgl::vector4 ambient_color;
		float fog_start;
		unsigned int fog_color;
		int u1;
	};
}
