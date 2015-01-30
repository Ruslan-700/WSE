#pragma once

#include "rgl.h"
#include "operation.h"

#define NUM_MAPPED_TABLEAUS 5

namespace wb
{
	struct tableau
	{
		rgl::scene tableau_scene;
		unsigned int override_flags;
		int num_item_overrides;
		int item_override_nos[10];
		rgl::camera camera;
		unsigned int background_color;
		rgl::vector4 ambient_light_color;
	};

	struct tableau_material
	{
		rgl::string id;
		int index;
		unsigned int flags;
		int width;
		int height;
		float u1;
		float u2;
		float mesh_min_x;
		float mesh_min_y;
		float mesh_max_x;
		float mesh_max_y;
		operation_manager operations;
		stl::vector<rgl::material *> materials;
		rgl::material *material;
	};

	struct tableau_manager
	{
		stl::vector<tableau_material> tableau_materials;
		stl::vector<tableau> tableaus;
		int render_cur_tableau_as_alpha_mask;
		int mapped_tableaus[NUM_MAPPED_TABLEAUS];
	};
}
