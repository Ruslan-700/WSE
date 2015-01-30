#include "rgl_scene.h"

using namespace rgl;

entity *scene::get_entity(int no) const
{
	int size = this->entities.size();

	for (int i = 0; i < size; ++i)
	{
		if (this->entities[i]->no == no)
			return this->entities[i];
	}

	size = this->queued_entities.size();

	for (int i = 0; i < size; ++i)
	{
		if (this->queued_entities[i]->no == no)
			return this->queued_entities[i];
	}

	return nullptr;
}

void scene::get_ground_data(const rgl::vector4 &position, float &height, rgl::quad **quad)
{
	THISCALL3(rgl::addresses::scene_GetGroundData, this, position, height, quad);
}

void scene::get_body_parts_in_path(const rgl::vector4 &start, const rgl::vector4 &end, unsigned int ignore_flags, rgl::body_part **body_parts, int &num_body_parts)
{
	THISCALL5(rgl::addresses::scene_GetBodyPartsInPath, this, start, end, ignore_flags, body_parts, num_body_parts);
}
