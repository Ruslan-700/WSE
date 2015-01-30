#include "WSETroopOperationsContext.h"

#include "WSE.h"
#include "warband.h"

int TroopGetSkillPoints(WSETroopOperationsContext *context)
{
	int troop_no;
	
	context->ExtractTroopNo(troop_no);

	return warband->cur_game->troops[troop_no].skill_points;
}

void TroopSetSkillPoints(WSETroopOperationsContext *context)
{
	int troop_no, points;
	
	context->ExtractTroopNo(troop_no);
	context->ExtractValue(points);

	if (points < 0)
		return;

	warband->cur_game->troops[troop_no].skill_points = points;
}

int TroopGetAttributePoints(WSETroopOperationsContext *context)
{
	int troop_no;
	
	context->ExtractTroopNo(troop_no);

	return warband->cur_game->troops[troop_no].attribute_points;
}

void TroopSetAttributePoints(WSETroopOperationsContext *context)
{
	int troop_no, points;
	
	context->ExtractTroopNo(troop_no);
	context->ExtractValue(points);

	if (points < 0)
		return;

	warband->cur_game->troops[troop_no].attribute_points = points;
}

int TroopGetProficiencyPoints(WSETroopOperationsContext *context)
{
	int troop_no;
	
	context->ExtractTroopNo(troop_no);

	return warband->cur_game->troops[troop_no].proficiency_points;
}

void TroopSetProficiencyPoints(WSETroopOperationsContext *context)
{
	int troop_no, points;
	
	context->ExtractTroopNo(troop_no);
	context->ExtractValue(points);

	if (points < 0)
		return;

	warband->cur_game->troops[troop_no].proficiency_points = points;
}

void TroopGetFaceKeys(WSETroopOperationsContext *context)
{
	int fkreg, troop_no;
	bool alt;

	context->ExtractRegister(fkreg);
	context->ExtractTroopNo(troop_no);
	context->ExtractBoolean(alt);

	WSE->SkinOperations.m_face_key_registers[fkreg] = alt ? warband->cur_game->troops[troop_no].face_keys_2 : warband->cur_game->troops[troop_no].face_keys_1;
}

void TroopSetFaceKeys(WSETroopOperationsContext *context)
{
	int troop_no, fkreg;
	bool alt;

	context->ExtractTroopNo(troop_no);
	context->ExtractRegister(fkreg);
	context->ExtractBoolean(alt);

	if (!alt)
		warband->cur_game->troops[troop_no].face_keys_1 = WSE->SkinOperations.m_face_key_registers[fkreg];
	else
		warband->cur_game->troops[troop_no].face_keys_2 = WSE->SkinOperations.m_face_key_registers[fkreg];
}

bool TroopHasFlag(WSEItemOperationsContext *context)
{
	int troop_no, flag;
	
	context->ExtractTroopNo(troop_no);
	context->ExtractValue(flag);
	
	return (warband->cur_game->troops[troop_no].flags & flag) > 0;
}

void TroopSetSkill(WSETroopOperationsContext *context)
{
	int troop_no, skill_no, value;
	
	context->ExtractTroopNo(troop_no);
	context->ExtractSkillNo(skill_no);
	context->ExtractValue(value);

	warband->cur_game->troops[troop_no].set_skill(skill_no, value);
}

void TroopSetAttribute(WSETroopOperationsContext *context)
{
	int troop_no, attribute, value;
	
	context->ExtractTroopNo(troop_no);
	context->ExtractBoundedValue(attribute, 0, NUM_TROOP_ATTRIBUTES);
	context->ExtractValue(value);

	warband->cur_game->troops[troop_no].set_attribute(attribute, value);
}

void TroopSetProficiency(WSETroopOperationsContext *context)
{
	int troop_no, proficiency, value;
	
	context->ExtractTroopNo(troop_no);
	context->ExtractBoundedValue(proficiency, 0, NUM_TROOP_PROFICIENCIES);
	context->ExtractClampedValue(value, 0, 1000);

	warband->cur_game->troops[troop_no].proficiencies[proficiency] = (float)value;
}

WSETroopOperationsContext::WSETroopOperationsContext() : WSEOperationContext("troop", 3700, 3799)
{
}

void WSETroopOperationsContext::OnLoad()
{
	RegisterOperation("troop_get_skill_points", TroopGetSkillPoints, Both, Lhs, 2, 2,
		"Stores <1>'s unused skill points into <0>",
		"destination", "troop_no");

	RegisterOperation("troop_set_skill_points", TroopSetSkillPoints, Both, None, 2, 2,
		"Sets <0>'s unused skill points to <1>",
		"troop_no", "value");

	RegisterOperation("troop_get_attribute_points", TroopGetAttributePoints, Both, Lhs, 2, 2,
		"Stores <1>'s unused attribute points into <0>",
		"destination", "troop_no");

	RegisterOperation("troop_set_attribute_points", TroopSetAttributePoints, Both, None, 2, 2,
		"Sets <0>'s unused attribute points to <1>",
		"troop_no", "value");

	RegisterOperation("troop_get_proficiency_points", TroopGetProficiencyPoints, Both, Lhs, 2, 2,
		"Stores <1>'s unused proficiency points into <0>",
		"destination", "troop_no");

	RegisterOperation("troop_set_proficiency_points", TroopSetProficiencyPoints, Both, None, 2, 2,
		"Sets <0>'s unused proficiency points to <1>",
		"troop_no", "value");
	
	RegisterOperation("troop_get_face_keys", TroopGetFaceKeys, Both, None, 2, 3,
		"Stores <1>'s face keys into <0>. if <2> is non-zero the second pair of face keys is stored",
		"face_key_register", "troop_no", "alt");
	
	RegisterOperation("troop_set_face_keys", TroopSetFaceKeys, Both, None, 2, 3,
		"Sets <0>'s face keys to <1>. if <2> is non-zero the second pair of face keys is set",
		"troop_no", "face_key_register", "alt");

	RegisterOperation("troop_has_flag", TroopHasFlag, Both, Cf, 2, 2,
		"Fails if <0> doesn't have <1>",
		"troop_no", "flag");

	RegisterOperation("troop_set_skill", TroopSetSkill, Both, None, 3, 3,
		"Sets <0>'s <1> to <2>",
		"troop_no", "skill_no", "value");

	RegisterOperation("troop_set_attribute", TroopSetAttribute, Both, None, 3, 3,
		"Sets <0>'s <1> to <2>",
		"troop_no", "attribute", "value");

	RegisterOperation("troop_set_proficiency", TroopSetProficiency, Both, None, 3, 3,
		"Sets <0>'s <1> to <2>",
		"troop_no", "proficiency", "value");
}
