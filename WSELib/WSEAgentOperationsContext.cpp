#include "WSEAgentOperationsContext.h"

#include "WSE.h"
#include "warband.h"

int AgentGetItemModifier(WSEAgentOperationsContext *context)
{
	int agent_no;
	
	context->ExtractAgentNo(agent_no);

	wb::agent *agent = &warband->cur_mission->agents[agent_no];

	return agent->type == wb::at_horse ? agent->horse_item.get_modifier() : -1;
}

int AgentGetItemSlotModifier(WSEAgentOperationsContext *context)
{
	int agent_no, slot_no;
	
	context->ExtractAgentNo(agent_no);
	context->ExtractBoundedValue(slot_no, 0, NUM_ITEM_SLOTS);

	return warband->cur_mission->agents[agent_no].items[slot_no].get_modifier();
}

int AgentGetAnimationProgress(WSEAgentOperationsContext *context)
{
	int agent_no, channel_no;
	
	context->ExtractAgentNo(agent_no);
	context->ExtractBoundedValue(channel_no, 0, 2);

	return (int)(warband->cur_mission->agents[agent_no].action_channels[channel_no].progress * 100.0f);
}

int AgentGetDna(WSEAgentOperationsContext *context)
{
	int agent_no;
	
	context->ExtractAgentNo(agent_no);

	return warband->cur_mission->agents[agent_no].dna;
}

int AgentGetGroundSceneProp(WSEAgentOperationsContext *context)
{
	int agent_no;

	context->ExtractAgentNo(agent_no);

	rgl::entity *entity = warband->cur_mission->agents[agent_no].get_ground_entity();

	if (!entity || entity->object_type != 12)
		return -1;

	wb::mission_object *object = &warband->cur_mission->mission_objects[entity->object_no];

	if (object->meta_type == wb::mt_scene_prop || object->meta_type == wb::mt_spawned_prop)
		return object->no;
	else
		return -1;
}

int AgentGetItemSlotAmmo(WSEAgentOperationsContext *context)
{
	int agent_no, item_slot_no;

	context->ExtractAgentNo(agent_no);
	context->ExtractBoundedValue(item_slot_no, 0, NUM_ITEM_SLOTS);

	return warband->cur_mission->agents[agent_no].items[item_slot_no].get_ammo();
}

void AgentSetItemSlotAmmo(WSEAgentOperationsContext *context)
{
	int agent_no, item_slot_no, value;

	context->ExtractAgentNo(agent_no);
	context->ExtractBoundedValue(item_slot_no, 0, NUM_ITEM_SLOTS);
	context->ExtractValue(value);

	warband->cur_mission->agents[agent_no].items[item_slot_no].set_ammo(value);
}

int AgentGetItemSlotHitPoints(WSEAgentOperationsContext *context)
{
	int agent_no, item_slot_no;

	context->ExtractAgentNo(agent_no);
	context->ExtractBoundedValue(item_slot_no, 0, NUM_ITEM_SLOTS);

	return warband->cur_mission->agents[agent_no].items[item_slot_no].get_health();
}

void AgentSetItemSlotHitPoints(WSEAgentOperationsContext *context)
{
	int agent_no, item_slot_no, value;

	context->ExtractAgentNo(agent_no);
	context->ExtractBoundedValue(item_slot_no, 0, NUM_ITEM_SLOTS);
	context->ExtractValue(value);

	warband->cur_mission->agents[agent_no].items[item_slot_no].set_health(value);
}

int AgentGetWieldedItemSlotNo(WSEAgentOperationsContext *context)
{
	int agent_no, hand_no;

	context->ExtractAgentNo(agent_no);
	context->ExtractBoundedValue(hand_no, 0, 2);

	return warband->cur_mission->agents[agent_no].wielded_items[hand_no];
}

void AgentGetBonePosition(WSEAgentOperationsContext *context)
{
	int preg, agent_no, bone_no;
	
	context->ExtractRegister(preg);
	context->ExtractAgentNo(agent_no);
	context->ExtractValue(bone_no);

	rgl::strategic_entity *entity = warband->cur_mission->agents[agent_no].entity;

	if (!entity)
		return;

	rgl::skeleton *skeleton = entity->skeleton;
	
	if (!skeleton || bone_no < 0 || bone_no >= skeleton->num_bones)
		return;

	warband->basic_game.position_registers[preg] = skeleton->get_bone_position(bone_no);
}

int AgentGetScale(WSEAgentOperationsContext *context)
{
	int agent_no;

	context->ExtractAgentNo(agent_no);

	return (int)(warband->cur_mission->agents[agent_no].scale * warband->basic_game.fixed_point_multiplier);
}

void AgentSetForcedLod(WSECoreOperationsContext *context)
{
	int agent_no, level;

	context->ExtractAgentNo(agent_no);
	context->ExtractBoundedValue(level, 0, 6);

	rgl::strategic_entity *entity = warband->cur_mission->agents[agent_no].entity;

	if (entity)
		entity->flags = (entity->flags & ~0xF0) | (level << 4);
}

int AgentGetItemSlotFlags(WSEAgentOperationsContext *context)
{
	int agent_no, item_slot_no;

	context->ExtractAgentNo(agent_no);
	context->ExtractBoundedValue(item_slot_no, 0, NUM_ITEM_SLOTS);

	return warband->cur_mission->agents[agent_no].items[item_slot_no].item_flags;
}

void AgentAiGetMoveTargetPosition(WSEAgentOperationsContext *context)
{
	int preg, agent_no;
	
	context->ExtractRegister(preg);
	context->ExtractAgentNo(agent_no);

	wb::agent *agent = &warband->cur_mission->agents[agent_no];
	
	warband->basic_game.position_registers[preg].o = agent->ai.target_position;
	warband->basic_game.position_registers[preg].rot.f = agent->ai.target_direction;
	warband->basic_game.position_registers[preg].rot.u = rgl::vector4(0.0f, 0.0f, 1.0f);
	warband->basic_game.position_registers[preg].orthonormalize();
	warband->basic_game.position_registers[preg].rotate_z(DEG2RAD(180));
}

void AgentSetHorse(WSEAgentOperationsContext *context)
{
	int agent_no, horse_agent_no;
	
	context->ExtractAgentNo(agent_no);
	context->ExtractValue(horse_agent_no);

	wb::agent *agent = &warband->cur_mission->agents[agent_no];

	if (warband->cur_mission->agents.is_valid_index(agent->horse_agent_no))
	{
		warband->cur_mission->agents[agent->horse_agent_no].rider_agent_no = -1;
		warband->cur_mission->agents[agent->horse_agent_no].ai.behavior = wb::aisb_hold;
		warband->cur_mission->agents[agent->horse_agent_no].ai.behavior_update_time = 10.0f;
		warband->cur_mission->agents[agent->horse_agent_no].ai.control.actions_2 = 0;
		warband->cur_mission->agents[agent->horse_agent_no].ai.behavior_update_timer.update();
	}

	if (horse_agent_no < 0)
	{
		agent->horse_agent_no = -1;
	}
	else if (warband->cur_mission->agents.is_valid_index(horse_agent_no))
	{
		warband->cur_mission->agents[horse_agent_no].rider_agent_no = agent_no;
		agent->horse_agent_no = horse_agent_no;
	}
}

void AgentAiSetSimpleBehavior(WSEAgentOperationsContext *context)
{
	int agent_no, behavior, time;
	
	context->ExtractAgentNo(agent_no);
	context->ExtractValue(behavior);
	context->ExtractValue(time);

	if (time <= 0)
		time = 10000000;

	wb::agent *agent = &warband->cur_mission->agents[agent_no];

	agent->set_ai_behavior(behavior);
	agent->ai.behavior_update_timer.update();
	agent->ai.behavior_update_time = (float)time;
}

void AgentAccelerate(WSEAgentOperationsContext *context)
{
	int agent_no, preg;
	
	context->ExtractAgentNo(agent_no);
	context->ExtractRegister(preg);

	wb::agent *agent = &warband->cur_mission->agents[agent_no];

	agent->movement_flags &= ~0x1;
	agent->acceleration += warband->basic_game.position_registers[preg].o;
}

void AgentSetItemSlotModifier(WSEAgentOperationsContext *context)
{
	int agent_no, slot_no, modifier_no;
	
	context->ExtractAgentNo(agent_no);
	context->ExtractBoundedValue(slot_no, 0, NUM_ITEM_SLOTS);
	context->ExtractItemModifierNo(modifier_no);

	warband->cur_mission->agents[agent_no].items[slot_no].set_modifier(modifier_no);
}

WSEAgentOperationsContext::WSEAgentOperationsContext() : WSEOperationContext("agent", 3300, 3399)
{
}

void WSEAgentOperationsContext::OnLoad()
{
	RegisterOperation("agent_get_item_modifier", AgentGetItemModifier, Both, Lhs, 2, 2,
		"Stores <1>'s horse item modifier (-1 if agent is not a horse) into <0>",
		"destination", "agent_no");

	RegisterOperation("agent_get_item_slot_modifier", AgentGetItemSlotModifier, Both, Lhs, 3, 3,
		"Stores <1>'s <2> modifier into <0>",
		"destination", "agent_no", "item_slot_no");

	RegisterOperation("agent_get_animation_progress", AgentGetAnimationProgress, Both, Lhs, 2, 3,
		"Stores <1>'s channel <2> animation progress (in %%) into <0>",
		"destination", "agent_no", "channel_no");

	RegisterOperation("agent_get_dna", AgentGetDna, Both, Lhs, 2, 2,
		"Stores <1>'s dna into <0>",
		"destination", "agent_no");

	RegisterOperation("agent_get_ground_scene_prop", AgentGetGroundSceneProp, Both, Lhs, 2, 2,
		"Stores the prop instance on which <1> is standing into <0>",
		"destination", "agent_no");

	RegisterOperation("agent_get_item_slot_ammo", AgentGetItemSlotAmmo, Both, Lhs, 3, 3,
		"Stores <1>'s <2> ammo count into <0>",
		"destination", "agent_no", "item_slot_no");

	RegisterOperation("agent_set_item_slot_ammo", AgentSetItemSlotAmmo, Both, None, 3, 3,
		"Sets <0>'s <1> ammo count to <2>",
		"agent_no", "item_slot_no", "value");

	RegisterOperation("agent_get_item_slot_hit_points", AgentGetItemSlotHitPoints, Both, Lhs, 3, 3,
		"Stores <1>'s <2> shield hitpoints into <0>",
		"destination", "agent_no", "item_slot_no");

	RegisterOperation("agent_set_item_slot_hit_points", AgentSetItemSlotHitPoints, Both, None, 3, 3,
		"Sets <0>'s <1> shield hitpoints to <2>",
		"agent_no", "item_slot_no", "value");

	RegisterOperation("agent_get_wielded_item_slot_no", AgentGetWieldedItemSlotNo, Both, Lhs, 2, 3,
		"Stores <1>'s wielded item slot for <2> into <0>",
		"destination", "agent_no", "hand_no");

	RegisterOperation("agent_get_bone_position", AgentGetBonePosition, Both, None, 3, 3,
		"Stores <1>'s <2> position into <0>",
		"position_register", "agent_no", "bone_no");

	RegisterOperation("agent_get_scale", AgentGetScale, Both, Lhs, 2, 2,
		"Stores <1>'s scale into <0>",
		"destination_fixed_point", "agent_no");

	RegisterOperation("agent_set_forced_lod", AgentSetForcedLod, Client, None, 2, 2,
		"Forces <0>'s LOD level to <1> (0 = auto)",
		"agent_no", "lod_level");

	RegisterOperation("agent_get_item_slot_flags", AgentGetItemSlotFlags, Both, Lhs, 3, 3,
		"Stores <1>'s <2> item slot flags into <0>",
		"destination", "agent_no", "item_slot_no");

	RegisterOperation("agent_ai_get_move_target_position", AgentAiGetMoveTargetPosition, Both, None, 2, 2,
		"Stores <1>'s move target position agent into <0>",
		"position_register", "agent_no");

	RegisterOperation("agent_set_horse", AgentSetHorse, Both, None, 2, 2,
		"Sets <0>'s horse to <1> (-1 for no horse)",
		"agent_no", "horse_agent_no");

	RegisterOperation("agent_ai_set_simple_behavior", AgentAiSetSimpleBehavior, Both, None, 2, 3,
		"Sets <0>'s behavior to <1> and guarantees it won't be changed for <2> seconds. If <2> is not specified or <= 0, it won't be changed until agent_force_rethink is called",
		"agent_no", "simple_behavior", "guaranteed_time");

	RegisterOperation("agent_accelerate", AgentAccelerate, Both, None, 2, 2,
		"Uses x, y, z components of <1> to apply acceleration to <0>",
		"agent_no", "position_register_no");

	RegisterOperation("agent_set_item_slot_modifier", AgentSetItemSlotModifier, Both, None, 3, 3,
		"Sets <0>'s <1> modifier to <2>",
		"agent_no", "item_slot_no", "item_modifier_no");
}
