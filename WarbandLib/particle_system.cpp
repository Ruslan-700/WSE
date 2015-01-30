#include "particle_system.h"

#include "warband.h"

using namespace wb;

rgl::particle_system *particle_system_manager::create(const int &particle_system_no, const rgl::vector4 &position, const int &u1)
{
	THISCALL3(wb::addresses::particle_system_manager_Create, this, particle_system_no, position, u1);
}
