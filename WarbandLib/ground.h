#pragma once

#include "rgl.h"

#define NUM_GROUND_SPECS 11

namespace wb
{
	struct ground_paint_layer
	{
		rgl::string ground_spec_name;
		int ground_spec_no;
		float *cells;
	};

	struct ground_paint
	{
		int num_layers;
		int ground_elevation_layer_no;
		int ground_leveling_layer_no;
		ground_paint_layer layers[NUM_GROUND_SPECS + 2];
		int size[2];
	};

	struct ground_spec
	{
		rgl::string id;
		rgl::material *material;
		rgl::material *multitex_material;
		rgl::vector4 color;
		float uv_scale;
		unsigned int flags;
	};
}
