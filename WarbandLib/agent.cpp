#include "agent.h"

#include "warband.h"

using namespace wb;

rgl::string agent::get_name() const
{
	if (this->player_no >= 0)
		return warband->multiplayer_data.players[this->player_no].name;

	if (this->horse_item.item_no >= 0)
		return warband->item_kinds[this->horse_item.item_no].name;

	if (this->troop_no >= 0)
		return warband->cur_game->troops[this->troop_no].name;

	return "unknown";
}

item &agent::get_wielded_item(item &itm, int hand)
{
	int wielded_item_id = this->wielded_items[hand];
	item *wielded_item = nullptr;

	if (wielded_item_id >= 0 && (hand == 0 || !this->both_hands_occupied))
		wielded_item = &this->items[wielded_item_id];

	itm.item_no = -1;

	if (wielded_item)
	{
		int wielded_item_kind_no = wielded_item->item_no;
		
		if (wielded_item_kind_no >= 0 && this->item_alternative_usages[wielded_item_id] && warband->item_kinds[wielded_item_kind_no].properties & 0x20000000)
			wielded_item_kind_no++;

		itm.item_no = wielded_item_kind_no;
		itm.item_flags = wielded_item->item_flags;
	}

	return itm;
}

void agent::set_first_person_body_meta_meshes_visibility(unsigned int visibility_flags)
{
#if defined WARBAND
	if (this->body_meta_meshes[bmm_head])
		this->body_meta_meshes[bmm_head]->visibility_flags = visibility_flags;

	if (this->body_meta_meshes[bmm_beard])
		this->body_meta_meshes[bmm_beard]->visibility_flags = visibility_flags;

	if (this->body_meta_meshes[bmm_hair])
		this->body_meta_meshes[bmm_hair]->visibility_flags = visibility_flags;

	if (this->body_meta_meshes[bmm_helmet])
		this->body_meta_meshes[bmm_helmet]->visibility_flags = visibility_flags;

	if (this->body_meta_meshes[bmm_unknown_2])
		this->body_meta_meshes[bmm_unknown_2]->visibility_flags = visibility_flags;
#endif
}

int agent::get_relation_with(agent &him) const
{
	mission &mission = *warband->cur_mission;
	int my_agent_no = this->no, his_agent_no = him.no, my_team_no = team_no, his_team_no = him.team_no;

	if (this->type == at_horse)
	{
		if (mission.agents.is_valid_index(this->rider_agent_no) && mission.agents[this->rider_agent_no].status == as_alive)
		{
			my_agent_no = mission.agents[this->rider_agent_no].no;
			my_team_no = mission.agents[this->rider_agent_no].team_no;
		}
		else
		{
			my_team_no = -1;
		}
	}

	if (him.type == at_horse)
	{
		if (mission.agents.is_valid_index(him.rider_agent_no) && mission.agents[him.rider_agent_no].status == as_alive)
		{
			his_agent_no = mission.agents[him.rider_agent_no].no;
			his_team_no = mission.agents[him.rider_agent_no].team_no;
		}
		else
		{
			his_team_no = -1;
		}
	}
	
	wb::agent &my_agent = mission.agents[my_agent_no];

	if (my_agent.has_relations)
	{
		int num_relations = my_agent.relation_agent_nos.size();

		for (int i = 0; i < num_relations; ++i)
		{
			if (my_agent.relation_agent_nos[i] == his_agent_no)
				return my_agent.relation_values[i];
		}
	}

	if (my_team_no < 0 || my_team_no >= MISSION_NUM_TEAMS || his_team_no < 0 || his_team_no >= MISSION_NUM_TEAMS)
		return 0;

	return mission.teams[my_team_no].relations[his_team_no];
}

bool agent::is_ally_with(agent *agent) const
{
	return get_relation_with(*agent) > 0;
}

bool agent::is_enemy_with(agent *agent) const
{
	return get_relation_with(*agent) < 0;
}

rgl::entity *agent::get_ground_entity() const
{
	if (!this->ground_quad)
		return nullptr;

	if (this->ground_quad->entity_no < 0)
		return nullptr;

	return warband->cur_mission->mission_scene->get_entity(this->ground_quad->entity_no);
}

bool agent::is_couching() const
{
	switch (action_channels[1].action_no)
	{
	case 132:
	case 133:
		return entity->skeleton->channels[1].u1 > 0.1f;
	}

	return false;
}

void agent::set_ai_behavior(int behavior)
{
	THISCALL1(wb::addresses::agent_SetAiBehavior, this, behavior);
}

void agent::add_stun(float duration)
{
	THISCALL1(wb::addresses::agent_AddStun, this, duration);
}

void agent::play_sound_at_position(bool *networkBroadcast, int type, bool *altSoundForPlayer, int *soundNo, int *playerSoundNo, const rgl::vector4 &position, float *frequency)
{
	int sound_type = type;
	THISCALL7(wb::addresses::agent_PlaySoundAtPosition, this, networkBroadcast, sound_type, altSoundForPlayer, soundNo, playerSoundNo, position, frequency);
}
