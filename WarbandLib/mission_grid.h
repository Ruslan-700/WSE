#pragma once

#include "rgl.h"

namespace wb
{
	struct agent;

	struct mission_grid_agent
	{
		mission_grid_agent *next;
		agent *agent;
	};

	struct mission_grid_location
	{
		int coords[2];
		mission_grid_agent *agent_obj;
	};

	struct mission_grid_iterator
	{
		int min_coords[2];
		int max_coords[2];
		int cur_coords[2];
		mission_grid_agent *agent_obj;
	};

	struct mission_grid
	{
		int required_size[2];
		int num_cells[2];
		float cell_size;
		float size[2];
		mission_grid_agent ***agent_objs;
		int cell_mask[2];
		rgl::pool<mission_grid_agent *> agent_pool;
		
		void get_cell_coordinates(int *coordinates, float *position);
		bool initialize_iterator(mission_grid_iterator &it, const rgl::vector4 &position, float radius);
		bool advance_iterator(mission_grid_iterator &it);
	};
}
