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
	//WSE->Hooks.HookFunction(this, wb::addresses::agent_ReceiveShieldHit_entry, AgentReceiveShieldHitHook);
	WSE->Hooks.HookFunction(this, wb::addresses::agent_GetScaleHuman_entry, AgentGetScaleHumanHook);
	WSE->Hooks.HookFunction(this, wb::addresses::agent_GetScaleHorse_entry, AgentGetScaleHorseHook);
	/*
	WSE->Hooks.HookFunction(this, wb::addresses::mission_object_Hit_entry, MissionObjectHitHook);
	WSE->Hooks.HookFunction(this, wb::addresses::agent_CancelSwing_entry, AgentGetItemForUnbalancedCheckHook);
	WSE->Hooks.HookFunction(this, wb::addresses::agent_DropItem_entry, AgentDropItemHook);
	*/
	WSE->Hooks.HookFunction(this, wb::addresses::agent_StartReloading_entry, AgentStartReloadingHook);
	WSE->Hooks.HookFunction(this, wb::addresses::agent_EndReloading_entry_1, AgentEndReloadingHook);
	WSE->Hooks.HookFunction(this, wb::addresses::agent_EndReloading_entry_2, AgentEndReloadingHook);
	WSE->Hooks.HookFunction(this, wb::addresses::mission_SpawnMissile_entry, MissionSpawnMissileHook);
	WSE->Hooks.HookFunction(this, wb::addresses::missile_Dive_entry, MissileDiveHook);
	WSE->Hooks.HookFunction(this, wb::addresses::item_Difficulty_entry, ItemDifficultyCheckHook);
	WSE->Hooks.HookFunction(this, wb::addresses::mission_object_WeaponKnockBack_entry, MissionObjectWeaponKnockBackHook);
	WSE->Hooks.HookFunction(this, wb::addresses::item_kind_ShieldNoParry_entry, ItemKindShieldNoParryHook);
	WSE->Hooks.HookFunction(this, wb::addresses::item_kind_ShieldNoParrySound_entry, ItemKindShieldNoParrySoundHook);
	WSE->Hooks.HookFunction(this, wb::addresses::item_kind_ShieldNoParryDamage_entry, ItemKindShieldNoParryDamageHook);
	WSE->Hooks.HookFunction(this, wb::addresses::item_kind_ShieldNoParryMissiles_entry, ItemKindShieldNoParryMissilesHook);
	WSE->Hooks.HookFunction(this, wb::addresses::item_kind_ShieldNoParrySpeed_entry, ItemKindShieldNoParrySpeedHook);
	WSE->Hooks.HookFunction(this, wb::addresses::item_kind_ShieldNoParryCouchedLance_entry, ItemKindShieldNoParryCouchedLanceHook);
	WSE->Hooks.HookFunction(this, wb::addresses::item_kind_DisableAgentSoundsHorseShort_entry, ItemKindDisableAgentSoundsHorseShortHook);
	WSE->Hooks.HookFunction(this, wb::addresses::agent_BlockedAttack_entry, AgentBlockedAttackHook);
	WSE->Hooks.HookFunction(this, wb::addresses::agent_Turn_entry, AgentTurnHook);
#if defined WARBAND
	WSE->Hooks.HookFunction(this, wb::addresses::UpdateHorseAgentEntityBody_entry, UpdateHorseAgentEntityBodyHook);
	WSE->Hooks.HookFunction(this, wb::addresses::tactical_window_ShowUseTooltip_entry, TacticalWindowShowUseTooltipHook);
	WSE->Hooks.HookFunction(this, wb::addresses::tactical_window_ShowCrosshair_entry, TacticalWindowShowCrosshairHook);
	WSE->Hooks.HookFunction(this, wb::addresses::item_kind_TransformHoldPosition_entry, ItemKindTransformHoldPositionHook);
	WSE->Hooks.HookFunction(this, wb::addresses::UpdateAgentEntityBody, UpdateAgentEntityBodyHook);
	WSE->Hooks.HookFunction(this, wb::addresses::item_kind_ShieldNoParryCarry_entry, ItemKindShieldNoParryCarryHook);
	WSE->Hooks.HookFunction(this, wb::addresses::agent_StartReloadingClient_entry, AgentStartReloadingClientHook);
	WSE->Hooks.HookFunction(this, wb::addresses::agent_BloodParticles_entry, AgentBloodParticlesHook);
#endif	
}

void WSEMissionContext::OnEvent(WSEContext *sender, WSEEvent evt, void *data)
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
		
		m_item_difficulty_attribute[0] = -1;
		m_item_difficulty_skill[0] = -1;
		m_item_difficulty_attribute[wb::itp_type_horse] = WSE->ModuleSettingsIni.Int("", "difficulty_attribute_itp_type_horse", -1);
		m_item_difficulty_skill[wb::itp_type_horse] = WSE->ModuleSettingsIni.Int("", "difficulty_skill_itp_type_horse", 24);
		m_item_difficulty_attribute[wb::itp_type_one_handed] = WSE->ModuleSettingsIni.Int("", "difficulty_attribute_itp_type_one_handed", 0);
		m_item_difficulty_skill[wb::itp_type_one_handed] = WSE->ModuleSettingsIni.Int("", "difficulty_skill_itp_type_one_handed", -1);
		m_item_difficulty_attribute[wb::itp_type_two_handed] = WSE->ModuleSettingsIni.Int("", "difficulty_attribute_itp_type_two_handed", 0);
		m_item_difficulty_skill[wb::itp_type_two_handed] = WSE->ModuleSettingsIni.Int("", "difficulty_skill_itp_type_two_handed", -1);
		m_item_difficulty_attribute[wb::itp_type_polearm] = WSE->ModuleSettingsIni.Int("", "difficulty_attribute_itp_type_polearm", 0);
		m_item_difficulty_skill[wb::itp_type_polearm] = WSE->ModuleSettingsIni.Int("", "difficulty_skill_itp_type_polearm", -1);
		m_item_difficulty_attribute[wb::itp_type_arrows] = WSE->ModuleSettingsIni.Int("", "difficulty_attribute_itp_type_arrows", -1);
		m_item_difficulty_skill[wb::itp_type_arrows] = WSE->ModuleSettingsIni.Int("", "difficulty_skill_itp_type_arrows", -1);
		m_item_difficulty_attribute[wb::itp_type_bolts] = WSE->ModuleSettingsIni.Int("", "difficulty_attribute_itp_type_bolts", -1);
		m_item_difficulty_skill[wb::itp_type_bolts] = WSE->ModuleSettingsIni.Int("", "difficulty_skill_itp_type_bolts", -1);
		m_item_difficulty_attribute[wb::itp_type_shield] = WSE->ModuleSettingsIni.Int("", "difficulty_attribute_itp_type_shield", -1);
		m_item_difficulty_skill[wb::itp_type_shield] = WSE->ModuleSettingsIni.Int("", "difficulty_skill_itp_type_shield", 26);
		m_item_difficulty_attribute[wb::itp_type_bow] = WSE->ModuleSettingsIni.Int("", "difficulty_attribute_itp_type_bow", -1);
		m_item_difficulty_skill[wb::itp_type_bow] = WSE->ModuleSettingsIni.Int("", "difficulty_skill_itp_type_bow", 33);
		m_item_difficulty_attribute[wb::itp_type_crossbow] = WSE->ModuleSettingsIni.Int("", "difficulty_attribute_itp_type_crossbow", 0);
		m_item_difficulty_skill[wb::itp_type_crossbow] = WSE->ModuleSettingsIni.Int("", "difficulty_skill_itp_type_crossbow", -1);
		m_item_difficulty_attribute[wb::itp_type_thrown] = WSE->ModuleSettingsIni.Int("", "difficulty_attribute_itp_type_thrown", -1);
		m_item_difficulty_skill[wb::itp_type_thrown] = WSE->ModuleSettingsIni.Int("", "difficulty_skill_itp_type_thrown", 34);
		m_item_difficulty_attribute[wb::itp_type_goods] = WSE->ModuleSettingsIni.Int("", "difficulty_attribute_itp_type_goods", -1);
		m_item_difficulty_skill[wb::itp_type_goods] = WSE->ModuleSettingsIni.Int("", "difficulty_skill_itp_type_goods", -1);
		m_item_difficulty_attribute[wb::itp_type_head_armor] = WSE->ModuleSettingsIni.Int("", "difficulty_attribute_itp_type_head_armor", 0);
		m_item_difficulty_skill[wb::itp_type_head_armor] = WSE->ModuleSettingsIni.Int("", "difficulty_skill_itp_type_head_armor", -1);
		m_item_difficulty_attribute[wb::itp_type_body_armor] = WSE->ModuleSettingsIni.Int("", "difficulty_attribute_itp_type_body_armor", 0);
		m_item_difficulty_skill[wb::itp_type_body_armor] = WSE->ModuleSettingsIni.Int("", "difficulty_skill_itp_type_body_armor", -1);
		m_item_difficulty_attribute[wb::itp_type_foot_armor] = WSE->ModuleSettingsIni.Int("", "difficulty_attribute_itp_type_foot_armor", 0);
		m_item_difficulty_skill[wb::itp_type_foot_armor] = WSE->ModuleSettingsIni.Int("", "difficulty_skill_itp_type_foot_armor", -1);
		m_item_difficulty_attribute[wb::itp_type_hand_armor] = WSE->ModuleSettingsIni.Int("", "difficulty_attribute_itp_type_hand_armor", 0);
		m_item_difficulty_skill[wb::itp_type_hand_armor] = WSE->ModuleSettingsIni.Int("", "difficulty_skill_itp_type_hand_armor", -1);
		m_item_difficulty_attribute[wb::itp_type_pistol] = WSE->ModuleSettingsIni.Int("", "difficulty_attribute_itp_type_pistol", -1);
		m_item_difficulty_skill[wb::itp_type_pistol] = WSE->ModuleSettingsIni.Int("", "difficulty_skill_itp_type_pistol", -1);
		m_item_difficulty_attribute[wb::itp_type_musket] = WSE->ModuleSettingsIni.Int("", "difficulty_attribute_itp_type_musket", -1);
		m_item_difficulty_skill[wb::itp_type_musket] = WSE->ModuleSettingsIni.Int("", "difficulty_skill_itp_type_musket", -1);
		m_item_difficulty_attribute[wb::itp_type_bullets] = WSE->ModuleSettingsIni.Int("", "difficulty_attribute_itp_type_bullets", -1);
		m_item_difficulty_skill[wb::itp_type_bullets] = WSE->ModuleSettingsIni.Int("", "difficulty_skill_itp_type_bullets", -1);
		m_item_difficulty_attribute[wb::itp_type_animal] = WSE->ModuleSettingsIni.Int("", "difficulty_attribute_itp_type_animal", -1);
		m_item_difficulty_skill[wb::itp_type_animal] = WSE->ModuleSettingsIni.Int("", "difficulty_skill_itp_type_animal", -1);
		m_item_difficulty_attribute[wb::itp_type_book] = WSE->ModuleSettingsIni.Int("", "difficulty_attribute_itp_type_book", -1);
		m_item_difficulty_skill[wb::itp_type_book] = WSE->ModuleSettingsIni.Int("", "difficulty_skill_itp_type_book", -1);

		break;
#if defined WARBAND
	case OnFrame:
		wb::mission *mission = warband->cur_mission;
		for (int i = 0; i < mission->agents.num_items; ++i)
		{
			if (mission->agents[i].valid)
			{
				wb::agent *agent = &mission->agents[i];
				if (agent->type == wb::at_human)
				{
					for (int j = wb::bmm_head; j < wb::bmm_name + 1; ++j)
					{
						if (agent->body_meta_meshes[j])
						{
							agent->body_meta_meshes[j]->deform_move();
						}
					}
				}
			}
		}

		for (int i = 0; i < mission->mission_objects.num_items; ++i)
		{
			if (mission->mission_objects[i].valid)
			{
				wb::mission_object *object = &mission->mission_objects[i];
				if ((object->meta_type == wb::mt_scene_prop || object->meta_type == wb::mt_spawned_prop) && object->entity && object->entity->meta_meshes.size())
				{
					rgl::meta_mesh *meta_mesh = object->entity->meta_meshes[0];

					if (meta_mesh->deformMode > 0)
					{
						warband->basic_game.trigger_param_1 = object->no;
						warband->basic_game.trigger_param_2 = (int)(meta_mesh->deformDuration * 1000.0f - ((warband->timers[2] / 100000.0f) - meta_mesh->deformStartTime) * 1000.0f);
						warband->scene_props[object->sub_kind_no].simple_triggers.execute(-108);
					}
				}
			}
		}
		break;
#endif	
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
	case -47:
	case -48:
	case -44:
	case -45:
	case -46:
	case -76:
	case -100:
		if (mission->mission_objects.is_valid_index(game->trigger_mission_object_no))
		{
			return mission->mission_objects[game->trigger_mission_object_no].entity;
		}
		break;
	case -50:
		if (mission->agents.is_valid_index(game->trigger_agent_no))
		{
			return mission->agents[game->trigger_agent_no].entity;
		}
		else if(mission->mission_objects.is_valid_index(game->trigger_mission_object_no))
		{
			return mission->mission_objects[game->trigger_mission_object_no].entity;
		}
		break;
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
	case -47:
	case -48:
	case -44:
	case -45:
	case -46:
	case -76:
	case -100:
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
	case -47:
	case -48:
	case -44:
	case -45:
	case -46:
	case -76:
	case -100:
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
	case -70:
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
	warband->basic_game.trigger_result = 0;
	warband->basic_game.result_string.clear();

	if (WSE->Game.HasScript(WSE_SCRIPT_CHAT_MESSAGE_RECEIVED))
	{
		warband->basic_game.string_registers[0] = *text;
		WSE->Game.ExecuteScript(WSE_SCRIPT_CHAT_MESSAGE_RECEIVED, 2, player, team_chat);
	}

	chatMessageReceivedEventData data;
	data.team_chat = team_chat;
	data.player = player;
	data.text = text;

	WSE->SendContextEvent(this, WSEEvent::OnChatMessageReceived, &data);

	if (warband->basic_game.trigger_result)
	{
		return false;

	}

	if (warband->basic_game.result_string.length() > 0)
	{
		*text = warband->basic_game.result_string;
	}

	return true;
}

bool WSEMissionContext::OnAgentApplyAttackRec(wb::agent *agent)
{
	wb::agent_blow *cur_blow = &agent->cur_blow;
	/*
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
	*/
	warband->basic_game.trigger_param_6 = static_cast<int>(std::round(cur_blow->raw_damage));
	warband->basic_game.trigger_param_7 = cur_blow->item.get_modifier();

	if (cur_blow->missile)
	{
		warband->basic_game.trigger_param_8 = cur_blow->missile->missile_item.get_modifier();
	}
	else
	{
		warband->basic_game.trigger_param_8 = -1;
	}

	WSE->Scripting.SetTriggerParam(9, cur_blow->damage_type);
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
	warband->basic_game.trigger_param_4 = missile->missile_item.item_no;
	warband->basic_game.trigger_param_5 = missile->missile_item.get_modifier();
	warband->basic_game.trigger_param_6 = missile->no;
	//WSE->Scripting.SetTriggerParam(4, missile->missile_item.item_no);
	//WSE->Scripting.SetTriggerParam(5, missile->missile_item.get_modifier());
	item_kind->simple_triggers.execute(-101);
	m_cur_missile = nullptr;
#if defined WARBAND
	warband->cur_game->trigger_meta_mesh = nullptr;
#endif
}

int WSEMissionContext::OnAgentShieldHit(wb::agent *agent, wb::item *shield_item, int raw_damage, int damage, wb::agent_blow *blow, wb::missile *missile)
{
	wb::item_kind *item_kind = &warband->item_kinds[shield_item->item_no];
	if (!item_kind->simple_triggers.has_trigger(-80))
		return damage;

	warband->basic_game.trigger_param_1 = agent->no;
	warband->basic_game.trigger_param_2 = blow->agent_no;
	warband->basic_game.trigger_param_3 = damage;
	warband->basic_game.trigger_param_4 = blow->item.item_no;
	warband->basic_game.trigger_param_6 = raw_damage;
	warband->basic_game.trigger_param_7 = blow->item.get_modifier();

	if (missile)
	{
		warband->basic_game.trigger_param_5 = missile->missile_item.item_no;
		warband->basic_game.trigger_param_8 = missile->missile_item.get_modifier();
	}
	else
	{
		warband->basic_game.trigger_param_5 = -1;
		warband->basic_game.trigger_param_8 = -1;
	}

	warband->basic_game.trigger_result = -1;
	item_kind->simple_triggers.execute(-80);

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
	WSE->SendContextEvent(this, OnWSEScriptEvent, (void*)agent);

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
	int morph_key = 10 * (skin->flags & 0x7);

	if (items[wb::is_foot].is_valid())
	{
		if (morph_key != 0)
		{
			if (meta_meshes[wb::bmm_left_foot])
				meta_meshes[wb::bmm_left_foot]->create_vertex_anim_morph((float)morph_key);

			if (meta_meshes[wb::bmm_right_foot])
				meta_meshes[wb::bmm_right_foot]->create_vertex_anim_morph((float)morph_key);
		}

		if (meta_meshes[wb::bmm_left_foot] && (!items[wb::is_body].is_valid() || (items[wb::is_body].get_item_kind()->properties & 0x800000) == 0))
		{
			wb::item_kind *item_kind = &warband->item_kinds[items[wb::is_foot].item_no];

			if (item_kind->simple_triggers.has_trigger(-50))
			{
				wb::game *game = warband->cur_game;

				game->trigger_mission_object_no = -1;
				game->trigger_agent_no = agent_no;
				game->trigger_item_slot_no = wb::is_foot;
				game->trigger_meta_mesh = meta_meshes[wb::bmm_left_foot];
				game->trigger_mesh = nullptr;
				warband->basic_game.trigger_param_1 = agent_no;
				warband->basic_game.trigger_param_2 = troop_no;
				item_kind->simple_triggers.execute(-50);
			}
		}
	}

	if (items[wb::is_body].is_valid() && items[wb::is_body].get_item_kind()->properties & 0x1000000000000000)
	{
		MBDeleteCharacterMetaMesh(entity, meta_meshes, wb::bmm_left_hand);
		MBDeleteCharacterMetaMesh(entity, meta_meshes, wb::bmm_right_hand);
		MBDeleteCharacterMetaMesh(entity, meta_meshes, wb::bmm_left_bracer);
		MBDeleteCharacterMetaMesh(entity, meta_meshes, wb::bmm_right_bracer);
	}

	/*
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
*/
#endif
}

bool WSEMissionContext::OnAgentDropItem(wb::agent *agent, int item_no)
{
	return agent->items[item_no].item_no >= 0;
}

void WSEMissionContext::OnAgentStartReloading(wb::agent *agent)
{
	warband->basic_game.trigger_param_1 = agent->no;
	warband->mission_templates[warband->cur_mission->cur_mission_template_no].triggers.execute(-105);
}

void WSEMissionContext::OnAgentEndReloading(wb::agent *agent)
{
	//if (agent->attack_action == 5)
	//{
		warband->basic_game.trigger_param_1 = agent->no;
		warband->mission_templates[warband->cur_mission->cur_mission_template_no].triggers.execute(-106);
	//}
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
	warband->cur_game->execute_script(52, missile->missile_item.item_no, missile->missile_item.get_modifier(), missile->shooting_item.item_no, missile->shooting_item.get_modifier(), missile->shooter_agent_no, missile->no);

	if (missile->missile_item.item_no >= 0)
	{
		wb::item_kind *item_kind = missile->missile_item.get_item_kind();
		
		warband->basic_game.position_registers[0] = pos;
		warband->basic_game.trigger_param_1 = missile->shooter_agent_no;
		warband->basic_game.trigger_param_2 = missile->shooting_item.item_no;
		warband->basic_game.trigger_param_3 = missile->shooting_item.get_modifier();
		warband->basic_game.trigger_param_4 = missile->missile_item.item_no;
		warband->basic_game.trigger_param_5 = missile->missile_item.get_modifier();
		warband->basic_game.trigger_param_6 = missile->no;

		item_kind->simple_triggers.execute(-104);
	}
}

void WSEMissionContext::OnItemDifficultyCheck(wb::item_kind &item_kind, int *attribute, int *skill)
{
	*attribute = -1;
	*skill = -1;

	int item_kind_type = item_kind.get_type();

	if (item_kind_type > 0 && item_kind_type <= wb::itp_type_book)
	{
		if (item_kind_type == wb::itp_type_shield && (item_kind.properties & 0x4000) > 0)
		{
			*attribute = m_item_difficulty_attribute[wb::itp_type_one_handed];
			*skill = m_item_difficulty_skill[wb::itp_type_one_handed];
		}
		else
		{
			*attribute = m_item_difficulty_attribute[item_kind_type];
			*skill = m_item_difficulty_skill[item_kind_type];
		}
	}
}

bool WSEMissionContext::OnMissionObjectWeaponKnockBack(wb::scene_prop *scene_prop)
{
	return (scene_prop->flags & 0x10000000) > 0;
}

bool WSEMissionContext::OnItemKindShieldNoParry(int item_no)
{
	if (item_no <= 0) return true;
	return (warband->item_kinds[item_no].properties & 0x4000) > 0;
}

bool WSEMissionContext::OnItemKindShieldNoParryCarry(wb::item_kind *item_kind)
{
	return (item_kind->get_type() == wb::itp_type_shield && (item_kind->properties & 0x4000) == 0);
}

bool WSEMissionContext::OnItemKindDisableAgentSounds(int item_no)
{
	return (warband->item_kinds[item_no].properties & 0x40000000000000) > 0;
}

void WSEMissionContext::OnAgentBlockedAttack(int agent_no, int item_no, wb::missile *missile, wb::agent *agent)
{
	warband->basic_game.trigger_param_1 = agent->no;
	warband->basic_game.trigger_param_2 = agent_no;
	warband->basic_game.trigger_param_3 = item_no;
	if (missile)
	{
		warband->basic_game.trigger_param_4 = missile->missile_item.item_no;
	}
	else
	{
		warband->basic_game.trigger_param_4 = -1;
	}
	
	warband->mission_templates[warband->cur_mission->cur_mission_template_no].triggers.execute(-103);
}

void WSEMissionContext::OnAgentTurn(wb::agent *agent, float *max_rotation_speed)
{
	if ((agent->action_channels[0].action_no >= 0 && agent->action_set->actions[agent->action_channels[0].action_no].flags & 0x800) || (agent->action_channels[1].action_no >= 0 && agent->action_set->actions[agent->action_channels[1].action_no].flags & 0x800))
		*max_rotation_speed = 0.0f;

	warband->basic_game.trigger_param_1 = agent->no;
	warband->basic_game.trigger_param_2 = static_cast<int>(std::round(*max_rotation_speed * warband->basic_game.fixed_point_multiplier));
	
	warband->basic_game.trigger_result = -1;

	warband->mission_templates[warband->cur_mission->cur_mission_template_no].triggers.execute(-102);

	if (warband->basic_game.trigger_result >= 0)
		*max_rotation_speed = warband->basic_game.trigger_result / (float)warband->basic_game.fixed_point_multiplier;
}

void WSEMissionContext::OnAgentGetBloodParticles(wb::agent *agent)
{
#if defined WARBAND
	if (warband->face_generator.num_skins)
	{
		if (agent->type == wb::at_horse)
		{
			if (WSE->Mission.m_item_horse_blood_particles.find(agent->horse_item.item_no) != WSE->Mission.m_item_horse_blood_particles.end())
			{
				warband->particle_system_manager.mapped_particle_systems[2] = WSE->Mission.m_item_horse_blood_particles[agent->horse_item.item_no].blood_particle_1_no;
				warband->particle_system_manager.mapped_particle_systems[3] = WSE->Mission.m_item_horse_blood_particles[agent->horse_item.item_no].blood_particle_2_no;
			}
			else
			{
				warband->particle_system_manager.mapped_particle_systems[2] = warband->face_generator.skins[0].blood_particle_1_no;
				warband->particle_system_manager.mapped_particle_systems[3] = warband->face_generator.skins[0].blood_particle_2_no;
			}
		}
		else
		{
			warband->particle_system_manager.mapped_particle_systems[2] = warband->face_generator.skins[agent->gender_no].blood_particle_1_no;
			warband->particle_system_manager.mapped_particle_systems[3] = warband->face_generator.skins[agent->gender_no].blood_particle_2_no;
		}	
	}
#endif
}

#if defined WARBAND
void WSEMissionContext::MBDeleteCharacterMetaMesh(rgl::strategic_entity *entity, rgl::meta_mesh **meta_meshes, int index)
{
	if (meta_meshes[index])
	{
		entity->skeleton->remove_meta_mesh(meta_meshes[index]);
		rgl::_delete(meta_meshes[index]);
		meta_meshes[index] = nullptr;
	}
}
#endif