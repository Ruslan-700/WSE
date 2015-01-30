#include "rgl_bone.h"

using namespace rgl;

void bone::remove_meta_mesh(meta_mesh *meta_mesh)
{
	this->meta_meshes.remove(meta_mesh);
}

void bone::remove_particle_system(int particle_system_no)
{
	for (int i = 0; i < this->meshes.size(); ++i)
	{
		this->meshes[i]->remove_particle_system(particle_system_no);
	}

	for (int i = 0; i < this->meta_meshes.size(); ++i)
	{
		this->meta_meshes[i]->remove_particle_system(particle_system_no);
	}
}
