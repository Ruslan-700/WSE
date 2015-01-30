#include "rgl_meta_mesh.h"

using namespace rgl;

meta_mesh_lod::~meta_mesh_lod()
{
	for (int i = 0; i < this->meshes.size(); ++i)
	{
		if (!this->meshes[i]->is_original)
			_delete(this->meshes[i]);
	}
}

void meta_mesh_lod::set_mesh_vertex_anim_frame_time(const float &time)
{
	for (int i = 0; i < this->meshes.size(); ++i)
	{
		this->meshes[i]->set_vertex_anim_frame_time(time);
	}
}

void meta_mesh_lod::set_base_mesh_vertex_anim_frame_time(const float &time)
{
	for (int i = 0; i < this->meshes.size(); ++i)
	{
		if (this->meshes[i]->base_mesh)
		{
			this->meshes[i]->base_mesh->set_vertex_anim_frame_time(time);
		}
	}
}

void meta_mesh_lod::create_vertex_anim_morph(const float &time)
{
	for (int i = 0; i < this->meshes.size(); ++i)
	{
		if (this->meshes[i]->vertex_keys.num_keys > 0 || (this->meshes[i]->base_mesh && this->meshes[i]->base_mesh->vertex_keys.num_keys > 0))
		{
			if (this->meshes[i]->cur_vertex_anim_frame_time < 0.0f)
				this->meshes[i]->create_vertex_anim_morph(time);
		}
	}
}

meta_mesh::meta_mesh()
{
	this->num_lods = 0;
	this->cur_lod = 0;
	this->visibility_flags = 0xFFFF;
	this->position.initialize();
}

meta_mesh *meta_mesh::create_copy() const
{
	meta_mesh *copy = _new<meta_mesh>();
	
	copy->mesh_prefix = this->mesh_prefix;
	copy->bounding_box = this->bounding_box;
	copy->num_lods = this->num_lods;
	copy->cur_lod = this->cur_lod;
	copy->position = this->position;
	copy->visibility_flags = this->visibility_flags;

	for (int i = 0; i < this->num_lods; ++i)
	{
		copy->lods[i].meshes.resize(this->lods[i].meshes.size());

		for (int j = 0; j < this->lods[i].meshes.size(); ++j)
		{
			copy->lods[i].meshes[j] = this->lods[i].meshes[j]->create_copy();
		}
	}

	return copy;
}

void meta_mesh::set_mesh_vertex_anim_frame_time(const float &time)
{
	for (int i = 0; i < this->num_lods; ++i)
	{
		this->lods[i].set_mesh_vertex_anim_frame_time(time);
	}
}

void meta_mesh::set_base_mesh_vertex_anim_frame_time(const float &time)
{
	for (int i = 0; i < this->num_lods; ++i)
	{
		this->lods[i].set_base_mesh_vertex_anim_frame_time(time);
	}
}

void meta_mesh::create_vertex_anim_morph(const float &time)
{
	for (int i = 0; i < this->num_lods; ++i)
	{
		this->lods[i].create_vertex_anim_morph(time);
	}
}

void meta_mesh::remove_particle_system(int particle_system_no)
{
	for (int i = 0; i < this->num_lods; ++i)
	{
		for (int j = 0; j < this->lods[i].meshes.size(); ++j)
		{
			this->lods[i].meshes[j]->remove_particle_system(particle_system_no);
		}
	}
}

void meta_mesh::set_vertex_color(unsigned int color)
{
	for (int i = 0; i < this->num_lods; ++i)
	{
		for (int j = 0; j < this->lods[i].meshes.size(); ++j)
		{
			this->lods[i].meshes[j]->set_vertex_color(color);
		}
	}
}
