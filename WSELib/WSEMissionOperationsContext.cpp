#include "WSEMissionOperationsContext.h"

#include "WSE.h"
#include "warband.h"

void PropInstanceReceiveDamage(WSEMissionOperationsContext *context)
{
	int mission_object_no, agent_no, damage;
	bool advanced;
	
	context->ExtractMissionObjectNo(mission_object_no);
	context->ExtractValue(agent_no);
	context->ExtractValue(damage);
	context->ExtractBoolean(advanced);

	if (agent_no != -1 && !warband->cur_mission->agents.is_valid_index(agent_no))
		return;

	wb::mission_object *mission_object = &warband->cur_mission->mission_objects[mission_object_no];

	if (mission_object->hit_points <= 0.0f || (mission_object->meta_type != wb::mt_scene_prop && mission_object->meta_type != wb::mt_spawned_prop))
		return;

	wb::scene_prop *scene_prop = &warband->scene_props[mission_object->sub_kind_no];
	
	if (advanced && !warband->basic_game.is_client() && scene_prop->simple_triggers.has_trigger(-42))
	{
		warband->cur_game->trigger_mission_object_no = mission_object_no;
		warband->basic_game.trigger_result = -1;
		warband->basic_game.trigger_param_1 = mission_object_no;
		warband->basic_game.trigger_param_2 = damage;
		warband->basic_game.trigger_param_3 = agent_no;
		WSE->Scripting.SetTriggerParam(4, -1);
		WSE->Scripting.SetTriggerParam(5, -1);
		WSE->Scripting.SetTriggerParam(6, -1);
		WSE->Scripting.SetTriggerParam(7, -1);
		warband->basic_game.position_registers[1] = warband->cur_game->cur_position;
		warband->basic_game.position_registers[2].o.x = (float)agent_no;
		scene_prop->simple_triggers.execute(-42);

		if (warband->basic_game.trigger_result >= 0)
			damage = (int)warband->basic_game.trigger_result;
	}

	mission_object->hit_points -= damage;

	if (mission_object->hit_points <= 0.0f)
	{
		for (int i = 0; i < mission_object->entity->static_body_parts.size(); ++i)
		{
			mission_object->entity->static_body_parts[i]->set_flags(0x2, true);
		}
	}

	if (advanced && warband->basic_game.is_server())
	{
		wb::multiplayer_event evt;
		wb::network_buffer nbuf;
		
		nbuf.pack_int32(mission_object_no, 14);
		nbuf.pack_int32(agent_no, 10, -1);
		nbuf.pack_matrix(warband->cur_game->cur_position, 17, -50.0f, 0.01f, 12, -1.01f, 0.0005f);
		nbuf.pack_int32((int)mission_object->hit_points, 14, -1000);
		nbuf.pack_int32(1, 1);
		evt.type = wb::mce_mission_object_hit;
		evt.add_buffer(nbuf);
		warband->network_manager.server.broadcast_event(evt);
	}

	if (scene_prop->flags & 0x8000 && !warband->basic_game.is_client() && mission_object->hit_points <= 0.0f)
	{
		warband->cur_game->trigger_mission_object_no = mission_object_no;
		warband->basic_game.trigger_param_1 = mission_object_no;
		warband->basic_game.trigger_param_2 = agent_no;
		scene_prop->simple_triggers.execute(-43);
	}
}

void AddPointLight(WSEMissionOperationsContext *context)
{
#if defined WARBAND
	int flicker_magnitude_int, flicker_interval_int;

	context->ExtractValue(flicker_magnitude_int);
	context->ExtractValue(flicker_interval_int);
	
	float flicker_magnitude = flicker_magnitude_int / 100.0f;
	float flicker_interval = flicker_interval_int / 100.0f;

	rgl::light *light = rgl::_new<rgl::light>();
	light->type = rgl::lt_point;
	light->diffuse_color = warband->cur_game->cur_color;
	light->specular_color = light->diffuse_color;
	light->local_position = warband->cur_game->cur_local_position;
	light->update_range();

	int trigger_no = context->GetCurrentTrigger();
	rgl::mesh *mesh = WSE->Mission.GetTriggerMesh(trigger_no);

	if (mesh)
	{
		mesh->set_light(light);
		return;
	}

	rgl::meta_mesh *meta_mesh = WSE->Mission.GetTriggerMetaMesh(trigger_no);

	if (meta_mesh && meta_mesh->lods[0].meshes.size())
	{
		meta_mesh->lods[0].meshes[0]->set_light(light);
		return;
	}
	
	rgl::strategic_entity *entity = WSE->Mission.GetTriggerEntity(trigger_no);
	int bone_no = WSE->Mission.GetTriggerBoneNo(context->GetCurrentTrigger());
	
	if (entity)
	{
		if (entity->skeleton && bone_no >= 0)
			entity->skeleton->bones[bone_no].set_light(light);
		else
			entity->set_light(light);

		return;
	}
	
	warband->cur_mission->mission_scene->lights.push_back(light);
	light->local_position = rgl::vector4::zero;
	light->position.transform(warband->cur_game->cur_local_position, warband->cur_game->cur_position);
#endif
}

void ParticleSystemAddNew(WSEMissionOperationsContext *context)
{
#if defined WARBAND
	int particle_system_no;
	
	context->ExtractParticleSystemNo(particle_system_no);
	
	rgl::particle_system *particle_system = warband->particle_system_manager.create(particle_system_no, warband->cur_game->cur_local_position);
	
	if (!particle_system)
		return;

	int trigger_no = context->GetCurrentTrigger();
	rgl::mesh *mesh = WSE->Mission.GetTriggerMesh(trigger_no);

	if (mesh)
	{
		mesh->particle_systems.push_back(particle_system);
		return;
	}

	rgl::meta_mesh *meta_mesh = WSE->Mission.GetTriggerMetaMesh(trigger_no);

	if (meta_mesh && meta_mesh->lods[0].meshes.size())
	{
		meta_mesh->lods[0].meshes[0]->particle_systems.push_back(particle_system);
		return;
	}
	
	rgl::strategic_entity *entity = WSE->Mission.GetTriggerEntity(trigger_no);
	int bone_no = WSE->Mission.GetTriggerBoneNo(trigger_no);
	
	if (entity)
	{
		if (entity->skeleton && bone_no >= 0)
			entity->skeleton->bones[bone_no].add_particle_system(particle_system);
		else
			entity->add_particle_system(particle_system);

		return;
	}

	particle_system->burst_position = warband->cur_game->cur_position;

	if (warband->cur_mission->mission_scene->num_particle_systems < warband->cur_mission->mission_scene->particle_systems.size())
		std::swap(particle_system, warband->cur_mission->mission_scene->particle_systems[warband->cur_mission->mission_scene->num_particle_systems]);

	warband->cur_mission->mission_scene->particle_systems.push_back(particle_system);
	warband->cur_mission->mission_scene->num_particle_systems++;
#endif
}

void MissionCamSetAperture(WSEMissionOperationsContext *context)
{
	int aperture;

	context->ExtractValue(aperture);

	wb::mission *mission = warband->cur_mission;

	if (!mission)
		return;

	mission->mission_cam_aperture = (float)aperture;
	mission->mission_cam_position_and_aperture_change_duration = 0.0f;
}

int StoreCurrentMissionTemplateNo(WSEMissionOperationsContext *context)
{
	if (!warband->cur_mission)
		return -1;

	return warband->cur_mission->cur_mission_template_no;
}

void MissionObjectClearAttachedMissiles(WSEMissionOperationsContext *context)
{
#if defined WARBAND
	int mission_object_no;
	
	context->ExtractMissionObjectNo(mission_object_no);

	wb::mission *mission = warband->cur_mission;
	wb::mission_object *mission_object = &mission->mission_objects[mission_object_no];

	if (!mission_object->entity || (mission_object->meta_type != wb::mt_scene_prop && mission_object->meta_type != wb::mt_spawned_prop))
		return;
	
	int num_attached_meta_meshes = mission->missiles_attached_mission_object_nos.size();
	stl::vector<int> attached_mission_object_nos;
	stl::vector<rgl::meta_mesh *> attached_meta_meshes;
	stl::vector<rgl::meta_mesh *> meta_meshes_to_remove;

	attached_mission_object_nos.reserve(num_attached_meta_meshes);
	attached_meta_meshes.reserve(num_attached_meta_meshes);

	for (int i = 0; i < num_attached_meta_meshes; ++i)
	{
		if (mission->missiles_attached_mission_object_nos[i] != mission_object_no)
		{
			attached_mission_object_nos.push_back(mission->missiles_attached_mission_object_nos[i]);
			attached_meta_meshes.push_back(mission->missiles_attached_meta_meshes[i]);
		}
		else
		{
			meta_meshes_to_remove.push_back(mission->missiles_attached_meta_meshes[i]);
		}
	}
	
	if (!meta_meshes_to_remove.size())
		return;

	mission->missiles_attached_mission_object_nos = attached_mission_object_nos;
	mission->missiles_attached_meta_meshes = attached_meta_meshes;

	rgl::strategic_entity *entity = mission_object->entity;
	stl::vector<rgl::meta_mesh *> meta_meshes_to_keep;
	int index = 0;

	for (int i = 0; i < meta_meshes_to_remove.size(); ++i)
	{
		while (entity->meta_meshes[index] != meta_meshes_to_remove[i] && index < entity->meta_meshes.size())
		{
			meta_meshes_to_keep.push_back(entity->meta_meshes[index]);
			index++;
		}

		rgl::_delete(meta_meshes_to_remove[i]);
		index++;
	}

	entity->meta_meshes = meta_meshes_to_keep;
#endif
}

bool CameraInFirstPerson(WSEMissionOperationsContext *context)
{
	if (!warband->cur_mission)
		return false;

	return warband->cur_game->camera_mode != 0;
}

void SetCameraInFirstPerson(WSEMissionOperationsContext *context)
{
	bool mode;

	context->ExtractBoolean(mode);

	wb::game *game = warband->cur_game;
	wb::mission *mission = warband->cur_mission;

	if (!mission || mission->player_agent_no < 0)
		return;

	wb::agent *agent = &mission->agents[mission->player_agent_no];

	game->camera_mode = mode ? 1 : 0;
	agent->set_first_person_body_meta_meshes_visibility(game->camera_mode ? 0x2000 : 0xFFFF);
	agent->control.camera_mode = game->camera_mode != 0;
}

void SetShowUseTooltip(WSEMissionOperationsContext *context)
{
	int type;
	bool value;

	context->ExtractBoundedValue(type, 0, 11);
	context->ExtractBoolean(value);

	WSE->Mission.m_use_objects_enabled[type] = value;
}

void MissionSetAllyCollisionThreshold(WSEMissionOperationsContext *context)
{
	int low, high;
	
	context->ExtractBoundedValue(low, 0, 101);
	context->ExtractBoundedValue(high, low, 101);

	WSE->Mission.m_ally_collision_threshold_low = low / 100.0f;
	WSE->Mission.m_ally_collision_threshold_high = high / 100.0f;
}

void MissionSetPropCollisionThreshold(WSEMissionOperationsContext *context)
{
	int dir, low, high;
	
	context->ExtractBoundedValue(dir, 0, 4);
	context->ExtractBoundedValue(low, 0, 101);
	context->ExtractBoundedValue(high, low, 101);
	
	WSE->Mission.m_prop_collision_threshold_low[dir] = low / 100.0f;
	WSE->Mission.m_prop_collision_threshold_high[dir] = high / 100.0f;
}

void ParticleSystemRemove(WSEMissionOperationsContext *context)
{
#if defined WARBAND
	int particle_system_no;
	
	context->ExtractParticleSystemNo(particle_system_no);
	
	int trigger_no = context->GetCurrentTrigger();
	rgl::mesh *mesh = WSE->Mission.GetTriggerMesh(trigger_no);

	if (mesh)
	{
		mesh->remove_particle_system(particle_system_no);
		return;
	}

	rgl::meta_mesh *meta_mesh = WSE->Mission.GetTriggerMetaMesh(trigger_no);

	if (meta_mesh)
	{
		meta_mesh->remove_particle_system(particle_system_no);
		return;
	}
	
	rgl::strategic_entity *entity = WSE->Mission.GetTriggerEntity(trigger_no);
	int bone_no = WSE->Mission.GetTriggerBoneNo(trigger_no);
	
	if (entity)
	{
		if (entity->skeleton && bone_no >= 0)
			entity->skeleton->bones[bone_no].remove_particle_system(particle_system_no);
		else
			entity->remove_particle_system(particle_system_no);
	}
#endif
}

void GetCameraPosition(WSEMissionOperationsContext *context)
{
	int preg;

	context->ExtractRegister(preg);

	warband->basic_game.position_registers[preg] = warband->cur_mission->camera_position;
}

int GetSpectatedAgentNo(WSEMissionOperationsContext *context)
{
#if defined WARBAND
	return ((wb::tactical_window *)warband->game_screen.game_windows[wb::gwt_tactical])->camera_agent_no;
#endif
	return -1;
}

void MissionObjectSetForcedLod(WSEMissionOperationsContext *context)
{
	int mission_object_no, level;

	context->ExtractMissionObjectNo(mission_object_no);
	context->ExtractBoundedValue(level, 0, 6);

	rgl::strategic_entity *entity = warband->cur_mission->mission_objects[mission_object_no].entity;

	if (entity)
		entity->flags = (entity->flags & ~0xF0) | (level << 4);
}

void MissionObjectSetVariationNo1(WSEMissionOperationsContext *context)
{
	int mission_object_no, value;

	context->ExtractMissionObjectNo(mission_object_no);
	context->ExtractValue(value);

	warband->cur_mission->mission_objects[mission_object_no].variation_no_1 = value;
}

void MissionObjectSetVariationNo2(WSEMissionOperationsContext *context)
{
	int mission_object_no, value;

	context->ExtractMissionObjectNo(mission_object_no);
	context->ExtractValue(value);

	warband->cur_mission->mission_objects[mission_object_no].variation_no_2 = value;
}

void StopTime(WSEMissionOperationsContext *context)
{
#if defined WARBAND
	bool value;
	
	context->ExtractBoolean(value);

	if (warband->config.cheat_mode)
		warband->cur_game->paused = value;
#endif
}

void CurMissileGetPathPointPosition(WSEMissionOperationsContext *context)
{
	int preg, path_point_no;

	context->ExtractRegister(preg);
	context->ExtractBoundedValue(path_point_no, 0, 500);

	wb::missile *missile = WSE->Mission.m_cur_missile;

	if (!missile)
		return;

	if (path_point_no == 0)
		warband->basic_game.position_registers[preg].rot.f = missile->starting_direction;
	else
		warband->basic_game.position_registers[preg].rot.f = ((missile->path[path_point_no] - missile->path[path_point_no - 1]) * 50.0f);

	warband->basic_game.position_registers[preg].rot.f.normalize();
	warband->basic_game.position_registers[preg].rot.u = rgl::vector4(0.0f, 1.0f, 0.0f);
	warband->basic_game.position_registers[preg].o = missile->path[path_point_no];
	warband->basic_game.position_registers[preg].orthonormalize();
}

int GetWaterLevel(WSEMissionOperationsContext *context)
{
	return (int)(warband->cur_mission->water_level * warband->basic_game.fixed_point_multiplier);
}

bool CastRay(WSEMissionOperationsContext *context)
{
	int preg1, preg2;
	float ray_length;
	
	context->ExtractRegister(preg1);
	context->ExtractRegister(preg2);
	context->ExtractFixedPoint(ray_length, 10000.0f);

	rgl::vector4 parent_dir;
	rgl::intersection_point point;
	rgl::vector4 ray_start_position = warband->basic_game.position_registers[preg2].o;
	rgl::vector4 ray_end_position = ray_start_position + warband->basic_game.position_registers[preg2].rot.f * ray_length;
	rgl::capsule ray_capsule(0.001f, ray_start_position, ray_end_position);
	rgl::body_part *body_parts[8192];
	int num_body_parts;
	warband->cur_mission->mission_scene->get_body_parts_in_path(ray_start_position, ray_end_position, rgl::bf_no_collision|rgl::bf_barrier_ai|rgl::bf_barrier_2d|rgl::bf_barrier_3d, body_parts, num_body_parts);
	float closest_distance = 10000000000.0f;
	rgl::body_part *closest_part = nullptr;
	rgl::vector4 closest_pos;
	rgl::vector4 closest_rot;
	int num_points;

	for (int i = 0; i < num_body_parts; ++i)
	{
		rgl::body_part *part = body_parts[i];

		num_points = 0;
		rgl::functions::check_intersection(&ray_capsule, part, 1, &point, num_points);

		if (num_points > 0)
		{
			float distance = (point.position - ray_start_position).length_squared();

			if (distance < closest_distance)
			{
				closest_distance = distance;
				closest_part = part;
				closest_pos = point.position;
				closest_rot = point.rotation;
			}
		}
	}

	if (closest_part)
	{
		if (closest_part->entity_no >= 0)
		{
			rgl::entity *entity = warband->cur_mission->mission_scene->get_entity(closest_part->entity_no);

			if (entity && entity->object_type == 12)
			{
				wb::mission_object *mission_object = &warband->cur_mission->mission_objects[entity->object_no];

				if (mission_object->meta_type == wb::mt_scene_prop || mission_object->meta_type == wb::mt_spawned_prop)
					context->SetReturnValue(entity->object_no);
				else
					context->SetReturnValue(-1);
			}
			else
			{
				context->SetReturnValue(-1);
			}
		}
		else
		{
			context->SetReturnValue(-1);
		}

		warband->basic_game.position_registers[preg1].o = closest_pos;
		warband->basic_game.position_registers[preg1].rot.f = closest_rot;
		warband->basic_game.position_registers[preg1].rot.u = rgl::vector4(0.0f, 0.0f, 1.0f);
		warband->basic_game.position_registers[preg1].orthonormalize();
		return true;
	}
	else
	{
		context->SetReturnValue(-1);
		return false;
	}
}

void MissionObjectSetVertexKeysTimePoint(WSEMissionOperationsContext *context)
{
	int mission_object_no, value;

	context->ExtractMissionObjectNo(mission_object_no);
	context->ExtractValue(value);

	wb::mission_object *mission_object = &warband->cur_mission->mission_objects[mission_object_no];

	if (!mission_object->entity)
		return;

	rgl::strategic_entity *entity = mission_object->entity;

#if !defined WARBAND_DEDICATED
	for (int i = 0; i < entity->meshes.size(); ++i)
	{
		entity->meshes[i]->set_vertex_anim_frame_time((float)value);
	}

	for (int i = 0; i < entity->meta_meshes.size(); ++i)
	{
		entity->meta_meshes[i]->set_mesh_vertex_anim_frame_time((float)value);
	}
#endif
}

void MissileRemoveOnHit(WSEMissionOperationsContext *context)
{
	if (context->GetCurrentTrigger() != wb::ti_on_missile_hit)
		return;

	int agent_no = (int)warband->basic_game.trigger_param_1;

	for (int i = warband->cur_mission->missiles.get_first_valid_index(); i < warband->cur_mission->missiles.size(); i = warband->cur_mission->missiles.get_next_valid_index(i))
	{
		wb::missile *missile = &warband->cur_mission->missiles[i];

		if (missile->shooter_agent_no == agent_no && (missile->hit_type == wb::mht_mission_object || missile->hit_type == wb::mht_scene))
		{
			missile->hit_type = wb::mht_scene_remove;

			if (missile->entity)
				missile->entity->visible = false;
		}
	}
}

WSEMissionOperationsContext::WSEMissionOperationsContext() : WSEOperationContext("mission", 3600, 3699)
{
}

void WSEMissionOperationsContext::OnLoad()
{
	ReplaceOperation(1877, "prop_instance_receive_damage", PropInstanceReceiveDamage, Both, None, 3, 4,
		"<0> received <2> damage from <1>. If <3> is non-zero ti_on_scene_prop_hit will be called and the damage dealt will be sent to clients.",
		"prop_instance_no", "agent_no", "damage", "advanced");

	ReplaceOperation(1960, "add_point_light", AddPointLight, Client, Undocumented, 0, 2,
		"Adds a point light with <0> and <1>",
		"flicker_magnitude", "flicker_interval");

	ReplaceOperation(1965, "particle_system_add_new", ParticleSystemAddNew, Client, Undocumented, 1, 1,
		"Adds <particle_system_no>",
		"particle_system_no");

	ReplaceOperation(2014, "mission_cam_set_aperture", MissionCamSetAperture, Client, Undocumented, 1, 1,
		"Changes the mission cam aperture",
		"aperture");

	RegisterOperation("store_cur_mission_template_no", StoreCurrentMissionTemplateNo, Both, Lhs, 1, 1,
		"Stores the current mission template into <0>",
		"destination");

	RegisterOperation("prop_instance_clear_attached_missiles", MissionObjectClearAttachedMissiles, Client, None, 1, 1,
		"Removes all attached missiles from <0>. Works only with dynamic scene props (non-retrievable missiles)",
		"prop_instance_no");

	RegisterOperation("camera_in_first_person", CameraInFirstPerson, Client, Cf, 0, 0,
		"Fails if the camera is not in first person");
	
	RegisterOperation("set_camera_in_first_person", SetCameraInFirstPerson, Client, None, 1, 1,
		"Sets the camera to first or third person",
		"value");

	RegisterOperation("set_show_use_tooltip", SetShowUseTooltip, Client, None, 1, 2,
		"Enables or disables use tooltips. See header_constants_addon.py for possible types",
		"tooltip_type", "value");

	RegisterOperation("set_ally_collision_threshold", MissionSetAllyCollisionThreshold, Both, None, 2, 2,
		"Changes the animation progress boundaries (in percents) that determine if attacks on allies will collide (default: 45% <= x <= 60%)",
		"low_boundary", "high_boundary");

	RegisterOperation("set_prop_collision_threshold", MissionSetPropCollisionThreshold, Both, None, 3, 3,
		"Changes the animation progress boundaries (in percents) that determine if swing attacks on props will collide (default: 40% <= x <= 80% (75% for overheads))",
		"attack_direction", "low_boundary", "high_boundary");

	RegisterOperation("particle_system_remove", ParticleSystemRemove, Client, None, 0, 1,
		"Removes <0> (all particle systems if not set or -1) from the current entity (can be used in several in triggers)",
		"particle_system_no");

	RegisterOperation("get_camera_position", GetCameraPosition, Client, None, 1, 1,
		"Stores camera position and rotation into <0>",
		"position_register_no");

	RegisterOperation("get_spectated_agent_no", GetSpectatedAgentNo, Client, Lhs, 1, 1,
		"Stores spectated agent no into <0>",
		"destination");

	RegisterOperation("prop_instance_set_forced_lod", MissionObjectSetForcedLod, Client, None, 2, 2,
		"Forces <0>'s LOD level to <1> (0 = auto)",
		"prop_instance_no", "lod_level");

	RegisterOperation("prop_instance_set_variation_id", MissionObjectSetVariationNo1, Both, None, 2, 2,
		"Sets <0>'s variation id to <1>",
		"prop_instance_no", "value");

	RegisterOperation("prop_instance_set_variation_id_2", MissionObjectSetVariationNo2, Both, None, 2, 2,
		"Sets <0>'s variation id 2 to <1>",
		"prop_instance_no", "value");

	RegisterOperation("stop_time", StopTime, Client, None, 1, 1,
		"Stops/resumes the mission. Works only in singleplayer with cheat mode enabled.",
		"value");

	RegisterOperation("cur_missile_get_path_point_position", CurMissileGetPathPointPosition, Both, None, 2, 2,
		"Stores the position of the missile's <1> (0-499) into <0> (can be used in ti_on_init_missile)",
		"position_register", "path_point_no");

	RegisterOperation("get_water_level", GetWaterLevel, Both, Lhs, 1, 1,
		"Stores the water level into <0>",
		"destination_fixed_point");
	
	RegisterOperation("cast_ray", CastRay, Both, Lhs|Cf, 3, 4,
		"Casts a ray of length <3> starting from <2> and stores the closest hit position into <1> (fails if no hits). If the body hit is a prop instance, its id will be stored into <0>",
		"destination", "hit_position_register", "ray_position_register", "ray_length_fixed_point");

	RegisterOperation("prop_instance_set_vertex_keys_time_point", MissionObjectSetVertexKeysTimePoint, Client, None, 2, 2,
		"Sets <0>'s mesh vertex keys time point to <1>",
		"prop_instance_no", "time_point");

	RegisterOperation("missile_remove_on_hit", MissileRemoveOnHit, Both, None, 0, 0,
		"Causes a missile item not to spawn on hit (can be only used inside ti_on_missile_hit)");
}
