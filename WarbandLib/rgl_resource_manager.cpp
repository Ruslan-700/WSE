#include "rgl_resource_manager.h"

using namespace rgl;

mesh *resource_manager::get_mesh(const string &name)
{
	if (!this->mesh_hashes_up_to_date)
	{
		for (int i = 0; i < this->meshes.size(); ++i)
		{
			if (this->meshes[i]->name == name)
				return this->meshes[i];
		}

		return nullptr;
	}

	int start = this->mesh_hashes[name[0]] - 1;
	int end = this->mesh_hashes[name[0] + 1] + 1;

	if (end < start)
		end = this->hashed_meshes.size() - 1;

	while (start < end - 1)
	{
		int cur = (end + start) >> 1;
		int cmp = strcmp(name.c_str(), this->hashed_meshes[cur]->name.c_str());

		if (cmp < 0)
			end = cur;
		else if (cmp > 0)
			start = cur;
		else
			return this->hashed_meshes[cur];
	}

	return nullptr;
}
