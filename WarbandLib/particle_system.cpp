#include "particle_system.h"

#include "warband.h"

using namespace wb;

rgl::particle_system *particle_system_manager::create(const int &particle_system_no, const rgl::vector4 &position, const int &u1)
{
	THISCALL3(wb::addresses::particle_system_manager_Create, this, particle_system_no, position, u1);
}

int particle_system_manager::add_system(const particle_system &system)
{
	particle_system *newSystems = rgl::_new<particle_system>(this->num_particle_systems + 1);

	for (int i = 0; i < this->num_particle_systems; ++i)
	{
		newSystems[i] = this->particle_systems[i];
	}
	rgl::_free(this->particle_systems);
	this->particle_systems = newSystems;
	this->particle_systems[this->num_particle_systems] = system;

	return this->num_particle_systems++;
}

bool particle_system_manager::remove_system(int index)
{
	if (index < 0)
		index += this->num_particle_systems;

	if (index < 0 || index >= this->num_particle_systems)
		return false;

	particle_system *newSystems = rgl::_new<particle_system>(this->num_particle_systems - 1);
	for (int i = 0; i < index; ++i)
	{
		newSystems[i] = this->particle_systems[i];
	}
	for (int i = index; i < this->num_particle_systems - 1; ++i)
	{
		newSystems[i] = this->particle_systems[i + 1];
	}
	rgl::_free(this->particle_systems);
	this->particle_systems = newSystems;
	this->num_particle_systems--;

	return true;
}