#pragma once

#include "rgl_matrix.h"
#include "rgl_vector4.h"

namespace rgl
{
	struct viewport
	{
		int x;
		int y;
		int width;
		int height;
		float min_z;
		float max_z;
	};

	struct camera_plane
	{
		vector4 position;
		vector4 rotation;
	};

	struct camera_frustum
	{
		int num_planes;
		camera_plane planes[6];
	};

	struct camera
	{
		bool perspective;
		bool left_handed;
		camera_frustum frustum;
		matrix position;
		float left_dist;
		float right_dist;
		float up_dist;
		float down_dist;
		float near_dist;
		float far_dist;
		float aperture;
		int u7;
	};
}
