#pragma once

#include "rgl_ai_mesh.h"
#include "rgl_entity.h"
#include "rgl_pool.h"

namespace rgl
{
	struct entity;

	struct scene_grid_entity
	{
		scene_grid_entity *next;
		scene_grid_entity *prev;
		entity *entity;
	};

	struct scene_grid_coordinate
	{
		scene_grid_coordinate *next;
		scene_grid_entity *entity_obj;
		int x;
		int y;
	};

	struct scene_grid_cell
	{
		stl::vector<body_part *> bodies_vec;
		stl::list<body_part *> bodies_list;
		stl::vector<ai_mesh_face *> ai_mesh_faces;
		scene_grid_entity *entity_obj;
	};

	struct scene_grid
	{
		int dim[2];
		int num_cells[2];
		float min[2];
		float max[2];
		float cell_size[2];
		scene_grid_cell **cells;
		pool<scene_grid_coordinate> coord_pool;
		pool<scene_grid_entity> entity_pool;
	};
}
