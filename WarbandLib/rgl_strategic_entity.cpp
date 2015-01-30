#if defined WARBAND
#include "rgl_strategic_entity.h"

using namespace rgl;

void strategic_entity::set_light(rgl::light *light)
{
	if (this->light)
		rgl::_delete(this->light);

	this->light = light;
}

void strategic_entity::remove_particle_system(int particle_system_no)
{
	stl::vector<particle_system *> removed_particle_systems;

	if (particle_system_no == -1)
	{
		removed_particle_systems = this->particle_systems;
		this->particle_systems.clear();
	}
	else
	{
		for (int i = 0; i < this->particle_systems.size(); ++i)
		{
			if (this->particle_systems[i]->particle_system_no == particle_system_no)
			{
				removed_particle_systems.push_back(this->particle_systems[i]);
				this->particle_systems.remove_at(i);
				i--;
			}
		}
	}

	for (int i = 0; i < removed_particle_systems.size(); ++i)
	{
		rgl::_delete(removed_particle_systems[i]);
	}
}
#endif
