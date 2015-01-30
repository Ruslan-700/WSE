#pragma once

#include "rgl_texture.h"

namespace rgl
{
	struct render_params
	{
		int u1;
		float fog_table_start;
		float fog_table_end;
		unsigned int fog_color;
		bool fog_enabled;
		float fog_density;
		float underwater_fog_table_start;
		float underwater_fog_table_end;
		unsigned int underwater_fog_color;
		float fog_water_level;
		float underwater_fog_density;
		bool shadowmap_enabled;
		texture *shadowmap_texture;
		camera shadowmap_camera;
		vector4 shadowmap_local_min;
		vector4 shadowmap_local_max;
		vector4 shadowmap_size;
		vector4 shadowmap_local_center;
		vector4 shadowmap_center;
		matrix shadowmap_transform_matrix;
		matrix shadowmap_camera_position;
	};
}
