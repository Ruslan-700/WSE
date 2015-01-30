#include "mission_grid.h"

#include "warband.h"

using namespace wb;

void mission_grid::get_cell_coordinates(int *coordinates, float *position)
{
	float x_pos = position[0] / this->cell_size;
	int x_coord;

	if (x_pos <= 0.0f)
		x_coord = (int)(x_pos - 0.9999999f);
	else
		x_coord = (int)(x_pos);

	coordinates[0] = x_coord & this->cell_mask[0];

	float y_pos = position[1] / this->cell_size;
	int y_coord;

	if (y_pos <= 0.0f)
		y_coord = (int)(y_pos - 0.9999999f);
	else
		y_coord = (int)(y_pos);

	coordinates[1] = y_coord & this->cell_mask[1];
}

bool mission_grid::initialize_iterator(mission_grid_iterator &it, const rgl::vector4 &position, float radius)
{
	float min_pos[2];
	float max_pos[2];
	int min_coords[2];
	int max_coords[2];
	
	min_pos[0] = position.x - radius;
	min_pos[1] = position.y - radius;
	max_pos[0] = position.x + radius;
	max_pos[1] = position.y + radius;
	
	this->get_cell_coordinates(min_coords, min_pos);
	this->get_cell_coordinates(max_coords, max_pos);
	
	it.min_coords[0] = min_coords[0];
	it.min_coords[1] = min_coords[1];
	it.max_coords[0] = max_coords[0];
	it.max_coords[1] = max_coords[1];
	it.cur_coords[0] = min_coords[0] - 1;
	it.cur_coords[1] = min_coords[1];
	it.agent_obj = nullptr;

	return this->advance_iterator(it);
}

bool mission_grid::advance_iterator(mission_grid_iterator &it)
{
	do
	{
		if (!it.agent_obj || !it.agent_obj->next)
		{
			if (it.cur_coords[0] != it.max_coords[0])
			{
				it.cur_coords[0] = (it.cur_coords[0] + 1) & this->cell_mask[0];
			}
			else
			{
				if (it.cur_coords[1] != it.max_coords[1])
				{
					it.cur_coords[0] = it.min_coords[0];
					it.cur_coords[1] = (it.cur_coords[1] + 1) & this->cell_mask[1];
				}
				else
				{
					it.agent_obj = nullptr;
					return false;
				}
			}
			
			it.agent_obj = this->agent_objs[it.cur_coords[0]][it.cur_coords[1]];
		}
		else
		{
			it.agent_obj = it.agent_obj->next;
		}
	}
	while (!it.agent_obj);

	return true;
}
