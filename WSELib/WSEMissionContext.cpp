#include "WSEMissionContext.h"

#include "WSE.h"

WSEMissionContext::WSEMissionContext()
{
	for (int i = 0; i < 11; ++i)
	{
		m_use_objects_enabled[i] = true;
	}

	m_cur_missile = nullptr;
}

void WSEMissionContext::OnLoad()
{
	WSE->Hooks.HookFunction(this, wb::addresses::mission_CheckHit_Human_entry, AgentAttackCollidesWithAllyHumanHook);
	WSE->Hooks.HookFunction(this, wb::addresses::mission_CheckHit_Horse_entry, AgentAttackCollidesWithAllyHorseHook);
	WSE->Hooks.HookFunction(this, wb::addresses::mission_CheckHit_Prop_entry, AgentAttackCollidesWithPropHook);
	WSE->Hooks.HookFunction(this, wb::addresses::agent_HorseCharged_entry, AgentHorseChargedHook);
	WSE->Hooks.HookFunction(this, wb::addresses::agent_ApplyAttackRecord_entry, AgentApplyAttackRecHook);
	WSE->Hooks.HookFunction(this, wb::addresses::agent_SetGroundQuad_entry, AgentSetGroundQuad);
	WSE->Hooks.HookFunction(this, wb::addresses::network_server_ReceiveChatEvent_entry, ChatMessageReceivedHook);
	WSE->Hooks.HookFunction(this, wb::addresses::network_server_ReceiveTeamChatEvent_entry, TeamChatMessageReceivedHook);
	WSE->Hooks.HookFunction(this, wb::addresses::agent_ReceiveShieldHit_entry, AgentReceiveShieldHitHook);
	WSE->Hooks.HookFunction(this, wb::addresses::agent_GetScaleHuman_entry, AgentGetScaleHumanHook);
	WSE->Hooks.HookFunction(this, wb::addresses::agent_GetScaleHorse_entry, AgentGetScaleHorseHook);
	WSE->Hooks.HookFunction(this, wb::addresses::mission_object_Hit_entry, MissionObjectHitHook);
	WSE->Hooks.HookFunction(this, wb::addresses::agent_CancelSwing_entry, AgentGetItemForUnbalancedCheckHook);
	WSE->Hooks.HookFunction(this, wb::addresses::agent_DropItem_entry, AgentDropItemHook);
	WSE->Hooks.HookFunction(this, wb::addresses::agent_StartReloading_entry, AgentStartReloadingHook);
	WSE->Hooks.HookFunction(this, wb::addresses::agent_EndReloading_entry_1, AgentEndReloadingHook);
	WSE->Hooks.HookFunction(this, wb::addresses::agent_EndReloading_entry_2, AgentEndReloadingHook);
	WSE->Hooks.HookFunction(this, wb::addresses::mission_SpawnMissile_entry, MissionSpawnMissileHook);
	WSE->Hooks.HookFunction(this, wb::addresses::missile_Dive_entry, MissileDiveHook);
#if defined WARBAND
	WSE->Hooks.HookFunction(this, wb::addresses::UpdateHorseAgentEntityBody_entry, UpdateHorseAgentEntityBodyHook);
	WSE->Hooks.HookFunction(this, wb::addresses::tactical_window_ShowUseTooltip_entry, TacticalWindowShowUseTooltipHook);
	WSE->Hooks.HookFunction(this, wb::addresses::tactical_window_ShowCrosshair_entry, TacticalWindowShowCrosshairHook);
	WSE->Hooks.HookFunction(this, wb::addresses::item_kind_TransformHoldPosition_entry, ItemKindTransformHoldPositionHook);
	WSE->Hooks.HookFunction(this, wb::addresses::UpdateAgentEntityBody, UpdateAgentEntityBodyHook);
#endif
}

void WSEMissionContext::OnEvent(WSEContext *sender, WSEEvent evt)
{
	switch (evt)
	{
	case ModuleLoad:
		m_ally_collision_threshold_low = 0.45f;
		m_ally_collision_threshold_high = 0.60f;
		m_prop_collision_threshold_low[0] = 0.40f;
		m_prop_collision_threshold_low[1] = 0.40f;
		m_prop_collision_threshold_low[2] = 0.40f;
		m_prop_collision_threshold_low[3] = 0.40f;
		m_prop_collision_threshold_high[0] = 0.80f;
		m_prop_collision_threshold_high[1] = 0.80f;
		m_prop_collision_threshold_high[2] = 0.80f;
		m_prop_collision_threshold_high[3] = 0.75f;
		WSE->Hooks.HookFunctionConditional(this, WSE->ModuleSettingsIni.Bool("", "ground_weapon_collision", false), wb::addresses::mission_CheckCollision_entry, MissionCheckCollisionHook);
		WSE->Hooks.HookFunctionConditional(this, WSE->ModuleSettingsIni.Bool("", "use_missile_damage_type", false), wb::addresses::mission_ApplyBlow_entry, MissionApplyBlowHook);
		break;
	}
}

rgl::strategic_entity *WSEMissionContext::GetTriggerEntity(int trigger_no) const
{
	wb::game *game = warband->cur_game;
	wb::mission *mission = warband->cur_mission;

	switch (trigger_no)
	{
	case -40:
	case -42:
	case -43:
		if (mission->mission_objects.is_valid_index(game->trigger_mission_object_no))
		{
			return mission->mission_objects[game->trigger_mission_object_no].entity;
		}
		break;
	case -50:
	case -51:
		if (mission->agents.is_valid_index(game->trigger_agent_no))
		{
			return mission->agents[game->trigger_agent_no].entity;
		}
		break;
	case -101:
	case -104:
		if (m_cur_missile)
		{
			return m_cur_missile->entity;
		}
		break;
	}

	return nullptr;
}

rgl::meta_mesh *WSEMissionContext::GetTriggerMetaMesh(int trigger_no) const
{
#if defined WARBAND
	wb::game *game = warband->cur_game;
	wb::mission *mission = warband->cur_mission;

	switch (trigger_no)
	{
	case -42:
	case -43:
		if (mission->mission_objects.is_valid_index(game->trigger_mission_object_no))
		{
			if (mission->mission_objects[game->trigger_mission_object_no].entity)
			{
				if (mission->mission_objects[game->trigger_mission_object_no].entity->meta_meshes.size())
					return mission->mission_objects[game->trigger_mission_object_no].entity->meta_meshes[0];
			}
		}
		break;
	case -40:
	case -50:
	case -51:
	case -101:
	case -104:
		return game->trigger_meta_mesh;
	}

#endif
	return nullptr;
}

rgl::mesh *WSEMissionContext::GetTriggerMesh(int trigger_no) const
{
#if defined WARBAND
	wb::game *game = warband->cur_game;
	wb::mission *mission = warband->cur_mission;

	switch (trigger_no)
	{
	case -42:
	case -43:
		if (mission->mission_objects.is_valid_index(game->trigger_mission_object_no))
		{
			if (mission->mission_objects[game->trigger_mission_object_no].entity)
			{
				if (mission->mission_objects[game->trigger_mission_object_no].entity->meshes.size())
					return mission->mission_objects[game->trigger_mission_object_no].entity->meshes[0];
			}
		}
		break;
	case -40:
	case -50:
		return game->trigger_mesh;
	}

#endif
	return nullptr;
}

int WSEMissionContext::GetTriggerBoneNo(int trigger_no) const
{
	wb::game *game = warband->cur_game;
	wb::mission *mission = warband->cur_mission;

	switch (trigger_no)
	{
	case -50:
	case -51:
		if (game->trigger_item_slot_no >= 0 && mission->agents.is_valid_index(game->trigger_agent_no))
		{
			int item_kind_no = mission->agents[game->trigger_agent_no].items[game->trigger_item_slot_no].item_no;

			if (item_kind_no >= 0)
				return warband->item_kinds[item_kind_no].get_attachment_bone_no();
		}
		break;
	}

	return -1;
}

bool WSEMissionContext::OnChatMessageReceived(bool team_chat, int player, rgl::string *text)
{
	if (!WSE->Game.HasScript(WSE_SCRIPT_CHAT_MESSAGE_RECEIVED))
		return true;

	warband->basic_game.string_registers[0] = *text;
	warband->basic_game.trigger_result = 0;
	warband->basic_game.result_string.clear();

	if (!WSE->Game.ExecuteScript(WSE_SCRIPT_CHAT_MESSAGE_RECEIVED, 2, player, team_chat))
		return true;

	if (warband->basic_game.trigger_result)
		return false;

	if (warband->basic_game.result_string.length() > 0)
		*text = warband->basic_game.result_string;
	
	return true;
}

bool WSEMissionContext::OnAgentApplyAttackRec(wb::agent *agent)
{
	wb::agent_blow *cur_blow = &agent->cur_blow;
	
	WSE->Scripting.SetTriggerParam(4, (int)cur_blow->raw_damage);
	WSE->Scripting.SetTriggerParam(5, cur_blow->hit_bone_no);
	WSE->Scripting.SetTriggerParam(6, cur_blow->item.item_no);
	WSE->Scripting.SetTriggerParam(7, cur_blow->item.get_modifier());

	if (cur_blow->missile)
	{
		WSE->Scripting.SetTriggerParam(8, cur_blow->missile->missile_item.item_no);
		WSE->Scripting.SetTriggerParam(9, cur_blow->missile->missile_item.get_modifier());
	}
	else
	{
		WSE->Scripting.SetTriggerParam(8, -1);
		WSE->Scripting.SetTriggerParam(9, -1);
	}

	WSE->Scripting.SetTriggerParam(10, cur_blow->damage_type);
	warband->mission_templates[warband->cur_mission->cur_mission_template_no].triggers.execute(-28);
	return true;
}

bool WSEMissionContext::OnAgentSetGroundQuad(wb::agent *agent, rgl::quad *quad)
{
	rgl::quad *old_quad = agent->ground_quad;
	
	agent->ground_quad = quad;

	if (!old_quad || !quad)
		return true;

	if (quad->entity_no >= 0 && old_quad->entity_no != quad->entity_no)
	{
		rgl::entity *entity = warband->cur_mission->mission_scene->get_entity(quad->entity_no);

		if (!entity)
			return true;

		if (entity->object_type == 12)
		{
			wb::mission_object *object = &warband->cur_mission->mission_objects[entity->object_no];

			if (object->meta_type == wb::mt_scene_prop || object->meta_type == wb::mt_spawned_prop)
			{
				warband->basic_game.trigger_param_1 = agent->no;
				warband->basic_game.trigger_param_2 = object->no;
				warband->scene_props[object->sub_kind_no].simple_triggers.execute(-100);
			}
		}
	}

	return true;
}

void WSEMissionContext::OnMissionSpawnMissile(wb::missile *missile)
{
	if (missile->missile_item.item_no < 0)
		return;

	wb::item_kind *item_kind = &warband->item_kinds[missile->missile_item.item_no];
	
	if (!item_kind->simple_triggers.has_trigger(-101))
		return;
	
	m_cur_missile = missile;
#if defined WARBAND
	warband->cur_game->trigger_meta_mesh = missile->entity->meta_meshes[0];
#endif
	warband->basic_game.trigger_param_1 = missile->shooter_agent_no;
	warband->basic_game.trigger_param_2 = missile->shooting_item.item_no;
	warband->basic_game.trigger_param_3 = missile->shooting_item.get_modifier();
	WSE->Scripting.SetTriggerParam(4, missile->missile_item.item_no);
	WSE->Scripting.SetTriggerParam(5, missile->missile_item.get_modifier());
	item_kind->simple_triggers.execute(-101);
	m_cur_missile = nullptr;
#if defined WARBAND
	warband->cur_game->trigger_meta_mesh = nullptr;
#endif
}

int WSEMissionContext::OnAgentShieldHit(wb::agent *agent, wb::item *shield_item, int raw_damage, int damage, wb::agent_blow *blow, wb::missile *missile)
{
	wb::item_kind *item_kind = &warband->item_kinds[shield_item->item_no];
	
	if (!item_kind->simple_triggers.has_trigger(-103))
		return damage;
	
	warband->basic_game.trigger_param_1 = agent->no;
	warband->basic_game.trigger_param_2 = blow->agent_no;
	warband->basic_game.trigger_param_3 = damage;
	WSE->Scripting.SetTriggerParam(4, raw_damage);
	WSE->Scripting.SetTriggerParam(5, blow->item.item_no);
	WSE->Scripting.SetTriggerParam(6, blow->item.get_modifier());

	if (missile)
	{
		WSE->Scripting.SetTriggerParam(7, missile->missile_item.item_no);
		WSE->Scripting.SetTriggerParam(8, missile->missile_item.get_modifier());
	}
	else
	{
		WSE->Scripting.SetTriggerParam(7, -1);
		WSE->Scripting.SetTriggerParam(8, -1);
	}

	warband->basic_game.trigger_result = -1;
	item_kind->simple_triggers.execute(-103);

	if (warband->basic_game.trigger_result >= 0)
		return (int)warband->basic_game.trigger_result;

	return damage;
}

void WSEMissionContext::OnUpdateHorseAgentEntityBody(int agent_no, wb::item *horse_item, rgl::meta_mesh *meta_mesh)
{
	wb::item_kind *item_kind = &warband->item_kinds[horse_item->item_no];

	if (!item_kind->simple_triggers.has_trigger(-50))
		return;
	
	wb::game *game = warband->cur_game;

	game->trigger_agent_no = agent_no;
	game->trigger_item_slot_no = -10000;
#if defined WARBAND
	game->trigger_meta_mesh = meta_mesh;
	game->trigger_mesh = nullptr;
#endif
	warband->basic_game.trigger_param_1 = agent_no;
	warband->basic_game.trigger_param_2 = -1;
	item_kind->simple_triggers.execute(-50);
}

void WSEMissionContext::OnShowUseTooltip(int object_type)
{
#if defined WARBAND
	if (object_type > 0 && !m_use_objects_enabled[object_type])
		warband->window_manager.set_tooltip_mesh(nullptr, true);
#endif
}

bool WSEMissionContext::OnAgentAttackCollidesWithAllyHuman(int agent_no)
{
	float progress = warband->cur_mission->agents[agent_no].action_channels[1].progress;

	return progress > m_ally_collision_threshold_low && progress < m_ally_collision_threshold_high;
}

bool WSEMissionContext::OnAgentAttackCollidesWithAllyHorse(int agent_no)
{
	float progress = warband->cur_mission->agents[agent_no].action_channels[1].progress;

	return progress > m_ally_collision_threshold_low && progress < m_ally_collision_threshold_high;
}

bool WSEMissionContext::OnAgentAttackCollidesWithProp(int agent_no, int attack_direction)
{
	float progress = warband->cur_mission->agents[agent_no].action_channels[1].progress;

	return progress > m_prop_collision_threshold_low[attack_direction] && progress < m_prop_collision_threshold_high[attack_direction];
}

bool WSEMissionContext::OnAgentHorseCharged(wb::agent *charger_agent, wb::agent *charged_agent)
{
	return charger_agent->is_enemy_with(charged_agent) || (warband->basic_game.is_multiplayer() && !warband->cur_mission->duel_mode && WSE->Network.GetHorseFriendlyFire() && charger_agent->controller != wb::ac_bot);
}

void WSEMissionContext::OnShowCrosshair()
{
#if defined WARBAND
	if (warband->basic_game.is_multiplayer() && !WSE->Network.GetShowCrosshair())
	{
		warband->cur_mission->crosshair_entities[0]->visible = false;
		warband->cur_mission->crosshair_entities[1]->visible = false;
		warband->cur_mission->crosshair_entities[2]->visible = false;
	}
#endif
}

void WSEMissionContext::OnMissionApplyBlow(wb::agent_blow *blow)
{
	if (blow->missile && blow->missile->missile_item.item_no >= 0)
		blow->damage_type = (warband->item_kinds[blow->missile->missile_item.item_no].thrust_damage >> 8) & 3;
}

void WSEMissionContext::OnAgentGetScale(wb::agent *agent)
{
	if (!WSE->Game.HasScript(WSE_SCRIPT_GET_AGENT_SCALE))
		return;

	warband->basic_game.trigger_result = 0;

	if (!WSE->Game.ExecuteScript(WSE_SCRIPT_GET_AGENT_SCALE, 4, agent->troop_no, agent->horse_item.item_no, agent->horse_item.get_modifier(), agent->player_no))
		return;

	if (warband->basic_game.trigger_result > 0)
		agent->scale = warband->basic_game.trigger_result / (float)warband->basic_game.fixed_point_multiplier;
}

void WSEMissionContext::OnMissionObjectHit(int agent_no, wb::item *item, wb::missile *missile)
{
	warband->basic_game.trigger_param_3 = agent_no;
	WSE->Scripting.SetTriggerParam(4, item->item_no);
	WSE->Scripting.SetTriggerParam(5, item->get_modifier());

	if (missile)
	{
		WSE->Scripting.SetTriggerParam(6, missile->missile_item.item_no);
		WSE->Scripting.SetTriggerParam(7, missile->missile_item.get_modifier());
	}
	else
	{
		WSE->Scripting.SetTriggerParam(6, -1);
		WSE->Scripting.SetTriggerParam(7, -1);
	}
}

void WSEMissionContext::OnItemKindTransformHoldPosition(wb::item_kind *item_kind, rgl::matrix *pos)
{
	pos->initialize();

	if ((item_kind->capabilities & 0xFF000) == 0x40000)
	{
		pos->rot.f = -pos->rot.f;
		pos->rot.s = -pos->rot.s;
	}
	else if (item_kind->properties & 0x0004000000000000)
	{
		if ((item_kind->capabilities & 0x7F0000000) == 0xA0000000)
		{
			pos->rot.f = -pos->rot.f;
			pos->rot.u = -pos->rot.u;
			pos->rotate_z(0.725f);
			pos->o = rgl::vector4(0.0f, item_kind->weapon_length * 0.01f, 0.0f);
		}
		else
		{
			pos->rot.f = -pos->rot.f;
			pos->rot.s = -pos->rot.s;
			pos->rotate_z(0.0365f);
			pos->o = rgl::vector4(0.0f, item_kind->weapon_length * 0.01f - 0.2f, 0.0f);
		}
	}
	else if (item_kind->properties & 0x1000000000000000)
	{
		pos->rot.f = -pos->rot.f;
		pos->rot.s = -pos->rot.s;
		pos->o = rgl::vector4(0.0f, item_kind->weapon_length * 0.01f - 0.55f, 0.0f);
	}
	else if (item_kind->properties & 0x4000000000000000)
	{
		pos->rot.s = -pos->rot.s;
		pos->rot.u = -pos->rot.u;
	}
}

wb::item *WSEMissionContext::OnAgentGetItemForUnbalancedCheck(wb::agent *agent)
{
	wb::item *item = nullptr;

	int wielded_item = agent->wielded_items[1];

	if (wielded_item >= 0)
	{
		if (agent->items[wielded_item].item_no >= 0 && !agent->both_hands_occupied)
			item = &agent->items[wielded_item];
	}

	if (!item)
	{
		wielded_item = agent->wielded_items[0];

		if (wielded_item >= 0)
		{
			if (agent->items[wielded_item].item_no >= 0)
				item = &agent->items[wielded_item];
		}
	}

	if (wielded_item >= 0 && agent->item_alternative_usages[wielded_item] && item && item->item_no >= 0 && (warband->item_kinds[item->item_no].properties & 0x20000000))
	{
		m_fake_item = *item;
		m_fake_item.item_no++;
		
		return &m_fake_item;
	}
	
	return item;
}

void WSEMissionContext::OnUpdateAgentEntityBody(rgl::strategic_entity *entity, int type, int troop_no, int agent_no, int player_no, rgl::meta_mesh **meta_meshes, wb::item *items)
{
#if defined WARBAND
	if (type != wb::at_human)
		return;

	wb::face_generator *face_gen = &warband->face_generator;
	wb::skin *skin = &face_gen->skins[face_gen->cur_skin_no];
	unsigned int skin_color = skin->face_textures[face_gen->cur_face_texture].color;
	
	if (items[wb::is_body].is_valid() && items[wb::is_body].get_item_kind()->properties & 0x40000000 && meta_meshes[wb::bmm_armor])
	{
		rgl::meta_mesh *body_meta_mesh = face_gen->skins[face_gen->cur_skin_no].body_meta_mesh;
		
		for (int i = 0; i < meta_meshes[wb::bmm_armor]->num_lods && i < body_meta_mesh->num_lods; ++i)
		{
			for (int j = 0; j < body_meta_mesh->lods[i].meshes.size(); ++j)
			{
				rgl::mesh *mesh = body_meta_mesh->lods[i].meshes[j]->create_copy();

				mesh->set_vertex_color(skin_color);
				meta_meshes[wb::bmm_armor]->lods[i].meshes.push_back(mesh);
			}
		}
	}

	if (items[wb::is_hand].is_valid() && items[wb::is_hand].get_item_kind()->properties & 0x40000000 && meta_meshes[wb::bmm_left_hand])
	{
		rgl::meta_mesh *left_hand_meta_mesh = face_gen->skins[face_gen->cur_skin_no].left_hand_meta_mesh;
		
		for (int i = 0; i < meta_meshes[wb::bmm_left_hand]->num_lods && i < left_hand_meta_mesh->num_lods; ++i)
		{
			for (int j = 0; j < left_hand_meta_mesh->lods[i].meshes.size(); ++j)
			{
				rgl::mesh *mesh = left_hand_meta_mesh->lods[i].meshes[j]->create_copy();
				
				if (!warband->hlsl_mode)
					mesh->set_vertex_color(skin_color);

				mesh->set_color(skin_color);
				meta_meshes[wb::bmm_left_hand]->lods[i].meshes.push_back(mesh);
			}
		}
	}

	if (items[wb::is_hand].is_valid() && items[wb::is_hand].get_item_kind()->properties & 0x80000000 && meta_meshes[wb::bmm_right_hand])
	{
		rgl::meta_mesh *right_hand_meta_mesh = face_gen->skins[face_gen->cur_skin_no].right_hand_meta_mesh;
		
		for (int i = 0; i < meta_meshes[wb::bmm_right_hand]->num_lods && i < right_hand_meta_mesh->num_lods; ++i)
		{
			for (int j = 0; j < right_hand_meta_mesh->lods[i].meshes.size(); ++j)
			{
				rgl::mesh *mesh = right_hand_meta_mesh->lods[i].meshes[j]->create_copy();
				
				if (!warband->hlsl_mode)
					mesh->set_vertex_color(skin_color);

				mesh->set_color(skin_color);
				meta_meshes[wb::bmm_right_hand]->lods[i].meshes.push_back(mesh);
			}
		}
	}

	int morph_key = 10 * (skin->flags & 0x7);
	
	if (morph_key != 0 && items[wb::is_foot].is_valid())
	{
		if (meta_meshes[wb::bmm_left_foot])
			meta_meshes[wb::bmm_left_foot]->create_vertex_anim_morph((float)morph_key);

		if (meta_meshes[wb::bmm_right_foot])
			meta_meshes[wb::bmm_right_foot]->create_vertex_anim_morph((float)morph_key);
	}

	if (morph_key != 0 && items[wb::is_head].is_valid())
	{
		if (meta_meshes[wb::bmm_helmet])
			meta_meshes[wb::bmm_helmet]->create_vertex_anim_morph((float)morph_key);
	}
#endif
}

bool WSEMissionContext::OnAgentDropItem(wb::agent *agent, int item_no)
{
	return agent->items[item_no].item_no >= 0;
}

void WSEMissionContext::OnAgentStartReloading(wb::agent *agent)
{
	WSE->Scripting.SetTriggerParam(1, agent->no);
	warband->mission_templates[warband->cur_mission->cur_mission_template_no].triggers.execute(-105);
}

void WSEMissionContext::OnAgentEndReloading(wb::agent *agent)
{
	if (agent->attack_action == 5)
	{
		WSE->Scripting.SetTriggerParam(1, agent->no);
		warband->mission_templates[warband->cur_mission->cur_mission_template_no].triggers.execute(-106);
	}
}

void WSEMissionContext::OnMissileDive(wb::missile *missile)
{
	rgl::matrix pos;
	
	pos.rot.f = missile->get_direction();
	pos.rot.u = rgl::vector4(0.0f, 0.0f, 1.0f);
	pos.orthonormalize();
	pos.o = missile->cur_position;
	pos.o.z = warband->cur_mission->water_level;

	warband->basic_game.position_registers[1] = pos;
	warband->cur_game->execute_script(52, missile->missile_item.item_no, missile->missile_item.get_modifier(), missile->shooting_item.item_no, missile->shooting_item.get_modifier(), missile->shooter_agent_no);

	if (missile->missile_item.item_no >= 0)
	{
		wb::item_kind *item_kind = missile->missile_item.get_item_kind();
		
		warband->basic_game.position_registers[0] = pos;
		WSE->Scripting.SetTriggerParam(1, missile->shooter_agent_no);
		WSE->Scripting.SetTriggerParam(2, missile->shooting_item.item_no);
		WSE->Scripting.SetTriggerParam(3, missile->shooting_item.get_modifier());
		WSE->Scripting.SetTriggerParam(4, missile->missile_item.item_no);
		WSE->Scripting.SetTriggerParam(5, missile->missile_item.get_modifier());
		item_kind->simple_triggers.execute(-104);
	}
}
