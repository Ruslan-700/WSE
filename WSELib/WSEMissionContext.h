#pragma once

#include "fmod.h"
#include "WSEContext.h"
#include "warband.h"
#include <map>

struct chatMessageReceivedEventData
{
	bool team_chat;
	int player;
	rgl::string *text;
};

struct bloodParticles
{
	int blood_particle_1_no;
	int blood_particle_2_no;
};

class WSEMissionContext : public WSEContext
{
public:
	WSEMissionContext();
	
protected:
	virtual void OnLoad();
	virtual void OnEvent(WSEContext *sender, WSEEvent evt, void *data);

public:
	rgl::strategic_entity *GetTriggerEntity(int trigger_no) const;
	rgl::meta_mesh *GetTriggerMetaMesh(int trigger_no) const;
	rgl::mesh *GetTriggerMesh(int trigger_no) const;
	int GetTriggerBoneNo(int trigger_no) const;
	
private:
	bool OnChatMessageReceived(bool team_chat, int player, rgl::string *text);
	bool OnAgentApplyAttackRec(wb::agent *agent);
	bool OnAgentSetGroundQuad(wb::agent *agent, rgl::quad *quad);
	void OnMissionSpawnMissile(wb::missile *missile);
	int OnAgentShieldHit(wb::agent *agent, wb::item *shield_item, int raw_damage, int damage, wb::agent_blow *blow, wb::missile *missile);
	void OnUpdateHorseAgentEntityBody(int agent_no, wb::item *horse_item, rgl::meta_mesh *meta_mesh);
	void OnShowUseTooltip(int object_type);
	bool OnAgentAttackCollidesWithAllyHuman(int agent_no);
	bool OnAgentAttackCollidesWithAllyHorse(int agent_no);
	bool OnAgentAttackCollidesWithProp(int agent_no, int attack_direction);
	bool OnAgentHorseCharged(wb::agent *charger_agent, wb::agent *charged_agent);
	void OnShowCrosshair();
	void OnMissionApplyBlow(wb::agent_blow *blow);
	void OnAgentGetScale(wb::agent *agent);
	void OnMissionObjectHit(int agent_no, wb::item *item, wb::missile *missile);
	void OnItemKindTransformHoldPosition(wb::item_kind *item_kind, rgl::matrix *pos);
	wb::item *OnAgentGetItemForUnbalancedCheck(wb::agent *agent);
	void OnUpdateAgentEntityBody(rgl::strategic_entity *entity, int type, int troop_no, int agent_no, int player_no, rgl::meta_mesh **meta_meshes, wb::item *items);
	bool OnAgentDropItem(wb::agent *agent, int item_no);
	void OnAgentStartReloading(wb::agent *agent);
	void OnAgentEndReloading(wb::agent *agent);
	void OnMissileDive(wb::missile *missile);
	void OnItemDifficultyCheck(wb::item_kind &item_kind, int *attribute, int *skill);
	bool OnMissionObjectWeaponKnockBack(wb::scene_prop *scene_prop);
	bool OnItemKindShieldNoParry(int item_no);
	bool OnItemKindShieldNoParryCarry(wb::item_kind *item_kind);
	bool OnItemKindDisableAgentSounds(int item_no);
	void OnAgentBlockedAttack(int agent_no, int item_no, wb::missile *missile, wb::agent *agent);
	void OnAgentTurn(wb::agent *agent, float *max_rotation_speed);
	void OnAgentGetBloodParticles(wb::agent *agent);
#if defined WARBAND
	void MBDeleteCharacterMetaMesh(rgl::strategic_entity *entity, rgl::meta_mesh **meta_meshes, int index);
#endif

public:
	wb::missile *m_cur_missile;
	bool m_use_objects_enabled[11];
	float m_ally_collision_threshold_low;
	float m_ally_collision_threshold_high;
	float m_prop_collision_threshold_low[4];
	float m_prop_collision_threshold_high[4];
	wb::item m_fake_item;
	int m_item_difficulty_attribute[21];
	int m_item_difficulty_skill[21];
	std::map <unsigned int, wb::action_set> m_agents_personal_action_manager;
	std::map <unsigned int, std::map <int,int>> m_agents_personal_animations;
	std::map <unsigned int, bloodParticles> m_item_horse_blood_particles;
};
