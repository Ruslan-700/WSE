#include "rgl_mesh.h"

using namespace rgl;

mesh *mesh::create_copy() const
{
	mesh *copy_mesh = nullptr;

	THISCALLRETN0(copy_mesh, addresses::mesh_CreateCopy, this);

	return copy_mesh;
}

void mesh::create_unique_vertex_data()
{
	THISCALL0(addresses::mesh_CreateUniqueVertexData, this);
}

void mesh::set_light(rgl::light *light)
{
	if (this->light)
		rgl::_delete(this->light);

	this->light = light;
}

void mesh::set_vertex_anim_frame_time(const float &time)
{
	mesh_vertex_keys *keys = get_vertex_keys();

	if (keys->num_keys > 0)
	{
		float max_time = (float)keys->time_points[keys->num_keys - 1];

		this->next_vertex_anim_frame_time = time < max_time ? time : max_time;
	}
}

void mesh::create_vertex_anim_morph(const float &time)
{
	THISCALL1(rgl::addresses::mesh_CreateVertexAnimMorph, this, time);
}

mesh_vertex_keys *mesh::get_vertex_keys()
{
	if (has_unique_vertex_data)
		return &this->vertex_keys;
	else
		return this->get_base_vertex_keys();
}

mesh_vertex_keys *mesh::get_base_vertex_keys()
{
	mesh *cur_mesh = this;

	while (!cur_mesh->has_unique_vertex_data)
	{
		cur_mesh = this->base_mesh;
	}

	return &cur_mesh->vertex_keys;
}

void mesh::remove_particle_system(int particle_system_no)
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

void mesh::set_vertex_color(unsigned int color)
{
	this->create_unique_vertex_data();

	for (int i = 0; i < this->patch_nodes.size(); ++i)
	{
		this->patch_nodes[i].color_1 = color;
	}

	this->buffer_update_recs[mbt_vertex_buffer].update = true;
}

void mesh::set_color(unsigned int color)
{
	material_color_1 = color;

	if ((material->flags & mf_alpha_blend_factor) == mf_alpha_blend_factor)
		render_order_is_up_to_date = false;
}

#if !defined WARBAND_DEDICATED
void mesh::replace_material(rgl::material *new_material)
{
	if (material == new_material)
		return;

	if (material)
		material->num_uses--;

	material = new_material;
	render_order_is_up_to_date = false;

	if (new_material)
		new_material->num_uses++;
}
#endif
