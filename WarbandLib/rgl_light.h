#pragma once

#include "rgl_vector4.h"

namespace rgl
{
	enum light_flags
	{
		lf_ambient   = 0x1,
		lf_no_shadow = 0x2,
	};

	enum light_type
	{
		lt_point       = 0,
		lt_spot        = 1,
		lt_directional = 2,
	};

	struct light
	{
		light_type type;
		vector4 diffuse_color;
		vector4 specular_color;
		vector4 position;
		vector4 local_position;
		vector4 direction;
		float range;
		float attenuation_0;
		float attenuation_1;
		float attenuation_2;
		float theta;
		float phi;
		unsigned int flags;
		float flicker_magnitude;
		float flicker_interval;
		int u1;

		light();
		void update_range();
	};
}
