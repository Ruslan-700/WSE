#include "rgl_meta_mesh.h"

#include "warband.h"

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

void meta_mesh_lod::add_mesh(mesh *mesh)
{
	meshes.push_back(mesh);
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

float meta_mesh::get_mesh_vertex_anim_frame_time()
{
	float next_vertex_anim_frame_time = 0.0f;

	for (int i = 0; i < lods[0].meshes.size(); ++i)
	{
		float temp_next_vertex_anim_frame_time = lods[0].meshes[i]->next_vertex_anim_frame_time;
		if (temp_next_vertex_anim_frame_time > 0.0f)
		{
			next_vertex_anim_frame_time = temp_next_vertex_anim_frame_time;
			break;
		}
	}

	return next_vertex_anim_frame_time;
}

void meta_mesh::start_deform_animation(int mode, float startFrame, float endFrame, float duration)
{
	if (duration >= 0.0f)
	{
		deformStartFrame = (startFrame > 0.0f) ? startFrame : 0.0f;
		deformEndFrame = (endFrame > 0.0f) ? endFrame : 0.0f;
		deformDuration = duration;
		deformMode = mode;
		deformStartTime = warband->timers[2] / 100000.0f;
	}
}

float meta_mesh::get_current_deform_animation_progress()
{
	if (deformMode <= 0)
		return 1.0f;

	float elapsedTime = (warband->timers[2] / 100000.0f) - deformStartTime;
	float progress;

	if (deformMode == 2)
	{
		float duration = deformDuration;
		while (elapsedTime >= duration)
		{
			elapsedTime -= duration;
		}
		progress = elapsedTime / duration;
	}
	else
	{
		progress = elapsedTime / deformDuration;
	}

	if (progress > 1.0f)
		return 1.0f;

	return progress;
}

void meta_mesh::deform_move()
{
	if (deformMode > 0)
	{
		float elapsedTime = (warband->timers[2] / 100000.0f) - deformStartTime;

		if (deformMode == 2)
		{
			while (elapsedTime >= deformDuration)
			{
				deformStartTime += deformDuration;
				elapsedTime = (warband->timers[2] / 100000.0f) - deformStartTime;
			}
		}

		float timePoint;
		if (elapsedTime <= 0.0f)
		{
			timePoint = deformStartFrame;
		}
		else
		{
			if (elapsedTime < deformDuration)
			{
				timePoint = elapsedTime * deformEndFrame / deformDuration + (deformDuration - elapsedTime) * deformStartFrame / deformDuration;
			}
			else
			{
				timePoint = deformEndFrame;
				deformMode = 0;
			}
		}

		set_mesh_vertex_anim_frame_time(timePoint);
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

void meta_mesh::set_visibility_flags(unsigned int visibility_flags)
{
	for (int i = 0; i < this->num_lods; ++i)
	{
		for (int j = 0; j < this->lods[i].meshes.size(); ++j)
		{
			this->lods[i].meshes[j]->visibility_flags = visibility_flags;
		}
	}
}

void meta_mesh::add_mesh(int lodNo, mesh *mesh)
{
	lods[lodNo].add_mesh(mesh);

	if (lodNo >= num_lods)
		num_lods = lodNo + 1;
}
