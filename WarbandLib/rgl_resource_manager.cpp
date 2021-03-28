#include "rgl_resource_manager.h"
#include "rgl_common.h"

using namespace rgl;

#if !defined WARBAND_DEDICATED

stl::vector<mesh *> resource_manager::get_meshes(const string &name)
{
	string meshName = name;

	stl::vector<mesh *> meshes;
	int nameLength = meshName.length();

	if (meshName.length() >= 2)
	{
		int start = mesh_hashes[meshName[0]];
		bool found = false;

		for (int i = start; i < hashed_meshes.size(); ++i)
		{
			mesh *mesh = hashed_meshes[i];

			if (mesh->name[0] != meshName[0])
				break;

			if (mesh->name[1] == meshName[1] && mesh->name.starts_with(meshName))
			{
				found = true;

				if (mesh->name.length() == nameLength || mesh->name[nameLength] == '.' && (mesh->name.length() > nameLength + 4 ||
					mesh->name[nameLength + 1] != 'l' && mesh->name[nameLength + 1] != 'L' ||
					mesh->name[nameLength + 2] != 'o' && mesh->name[nameLength + 2] != 'O' ||
					mesh->name[nameLength + 3] != 'd' && mesh->name[nameLength + 3] != 'D'))
				{
					meshes.push_back(mesh);
				}
			}
			else if (found)
			{
				break;
			}
		}
	}
	else
	{
		rglWarning("Incorrect base name: %s", meshName.c_str());
	}

	return meshes;
}

mesh *resource_manager::try_get_mesh(const string &name)
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

material *resource_manager::try_get_material(const string &name)
{
	for (int i = 0; i < materials.size(); ++i)
	{
		if (materials[i]->name == name)
			return materials[i];
	}

	return nullptr;
}

material *resource_manager::get_material(const string &name)
{
	material *material = try_get_material(name);

	if (material)
		return material;

	rglWarning("Unable to find material %s", name.c_str());

	if (name != "default")
		return get_default_material();

	rglWarning("Unable to find default material");
	return materials[0];
}

material *resource_manager::get_default_material()
{
	if (!def_material)
		def_material = get_material("default");

	return def_material;
}

#endif
