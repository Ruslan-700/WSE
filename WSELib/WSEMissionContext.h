#pragma once

#include "fmod.h"
#include "WSEContext.h"
#include "warband.h"

class WSEMissionContext : public WSEContext
{
public:
	WSEMissionContext();
	
protected:
	virtual void OnLoad();
	virtual void OnEvent(WSEContext *sender, WSEEvent evt);

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

public:
	wb::missile *m_cur_missile;
	bool m_use_objects_enabled[11];
	float m_ally_collision_threshold_low;
	float m_ally_collision_threshold_high;
	float m_prop_collision_threshold_low[4];
	float m_prop_collision_threshold_high[4];
	wb::item m_fake_item;
};
