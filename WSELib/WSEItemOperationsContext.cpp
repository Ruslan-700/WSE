#include "WSEItemOperationsContext.h"

#include "WSE.h"
#include "warband.h"

int ItemGetWeight(WSEItemOperationsContext *context)
{
	int item_kind_no;

	context->ExtractItemKindNo(item_kind_no);

	return (int)(warband->item_kinds[item_kind_no].weight * warband->basic_game.fixed_point_multiplier);
}

int ItemGetDifficulty(WSEItemOperationsContext *context)
{
	int item_kind_no;

	context->ExtractItemKindNo(item_kind_no);

	return warband->item_kinds[item_kind_no].difficulty;
}

int ItemGetValue(WSEItemOperationsContext *context)
{
	int item_kind_no;
	
	context->ExtractItemKindNo(item_kind_no);
	
	return warband->item_kinds[item_kind_no].value;
}

int ItemGetHeadArmor(WSEItemOperationsContext *context)
{
	int item_kind_no;
	
	context->ExtractItemKindNo(item_kind_no);
	
	return warband->item_kinds[item_kind_no].head_armor;
}

int ItemGetBodyArmor(WSEItemOperationsContext *context)
{
	int item_kind_no;
	
	context->ExtractItemKindNo(item_kind_no);
	
	return warband->item_kinds[item_kind_no].body_armor;
}

int ItemGetLegArmor(WSEItemOperationsContext *context)
{
	int item_kind_no;
	
	context->ExtractItemKindNo(item_kind_no);
	
	return warband->item_kinds[item_kind_no].leg_armor;
}

int ItemGetHitPoints(WSEItemOperationsContext *context)
{
	int item_kind_no;
	
	context->ExtractItemKindNo(item_kind_no);
	
	return warband->item_kinds[item_kind_no].hit_points;
}

int ItemGetSpeedRating(WSEItemOperationsContext *context)
{
	int item_kind_no;
	
	context->ExtractItemKindNo(item_kind_no);
	
	return warband->item_kinds[item_kind_no].speed_rating;
}

int ItemGetMissileSpeed(WSEItemOperationsContext *context)
{
	int item_kind_no;
	
	context->ExtractItemKindNo(item_kind_no);
	
	return warband->item_kinds[item_kind_no].missile_speed;
}

int ItemGetWeaponLength(WSEItemOperationsContext *context)
{
	int item_kind_no;
	
	context->ExtractItemKindNo(item_kind_no);
	
	return warband->item_kinds[item_kind_no].weapon_length;
}

int ItemGetMaxAmmo(WSEItemOperationsContext *context)
{
	int item_kind_no;
	
	context->ExtractItemKindNo(item_kind_no);
	
	return warband->item_kinds[item_kind_no].max_ammo;
}

int ItemGetAbundance(WSEItemOperationsContext *context)
{
	int item_kind_no;
	
	context->ExtractItemKindNo(item_kind_no);
	
	return warband->item_kinds[item_kind_no].abundance;
}

int ItemGetThrustDamage(WSEItemOperationsContext *context)
{
	int item_kind_no;
	
	context->ExtractItemKindNo(item_kind_no);
	
	return warband->item_kinds[item_kind_no].thrust_damage & 0xFF;
}

int ItemGetThrustDamageType(WSEItemOperationsContext *context)
{
	int item_kind_no;
	
	context->ExtractItemKindNo(item_kind_no);
	
	return warband->item_kinds[item_kind_no].thrust_damage >> 8;
}

int ItemGetSwingDamage(WSEItemOperationsContext *context)
{
	int item_kind_no;
	
	context->ExtractItemKindNo(item_kind_no);
	
	return warband->item_kinds[item_kind_no].swing_damage & 0xFF;
}

int ItemGetSwingDamageType(WSEItemOperationsContext *context)
{
	int item_kind_no;
	
	context->ExtractItemKindNo(item_kind_no);
	
	return warband->item_kinds[item_kind_no].swing_damage >> 8;
}

bool ItemHasProperty(WSEItemOperationsContext *context)
{
	int item_kind_no;
	__int64 item_property;
	
	context->ExtractItemKindNo(item_kind_no);
	context->ExtractBigValue(item_property);
	
	return (warband->item_kinds[item_kind_no].properties & item_property) > 0;
}

bool ItemHasCapability(WSEItemOperationsContext *context)
{
	int item_kind_no;
	__int64 capability;
	
	context->ExtractItemKindNo(item_kind_no);
	context->ExtractBigValue(capability);
	
	return (warband->item_kinds[item_kind_no].capabilities & capability) > 0;
}

bool ItemHasModifier(WSEItemOperationsContext *context)
{
	int item_kind_no;
	int item_modifier_no;
	
	context->ExtractItemKindNo(item_kind_no);
	context->ExtractItemModifierNo(item_modifier_no);
	
	return (warband->item_kinds[item_kind_no].modifiers >> item_modifier_no & 1) > 0;
}

bool ItemHasFaction(WSEItemOperationsContext *context)
{
	int item_kind_no, faction_no;
	
	context->ExtractItemKindNo(item_kind_no);
	context->ExtractFactionNo(faction_no);
	
	for (int i = 0; i < warband->item_kinds[item_kind_no].num_faction_nos; ++i)
	{
		if (warband->item_kinds[item_kind_no].faction_nos[i] == faction_no)
			return true;
	}

	return false;
}

void ItemSetThrustDamage(WSEItemOperationsContext *context)
{
	int item_kind_no;
	int value;
	
	context->ExtractItemKindNo(item_kind_no);
	context->ExtractValue(value);
	
	warband->item_kinds[item_kind_no].thrust_damage &= ~0xFF;
	warband->item_kinds[item_kind_no].thrust_damage |= value & 0xFF;
}

void ItemSetThrustDamageType(WSEItemOperationsContext *context)
{
	int item_kind_no;
	int value;
	
	context->ExtractItemKindNo(item_kind_no);
	context->ExtractValue(value);
	
	warband->item_kinds[item_kind_no].thrust_damage &= 0xFF;
	warband->item_kinds[item_kind_no].thrust_damage |= value << 8;
}

void ItemSetSwingDamage(WSEItemOperationsContext *context)
{
	int item_kind_no;
	int value;
	
	context->ExtractItemKindNo(item_kind_no);
	context->ExtractValue(value);
	
	warband->item_kinds[item_kind_no].swing_damage &= ~0xFF;
	warband->item_kinds[item_kind_no].swing_damage |= value & 0xFF;
}

void ItemSetSwingDamageType(WSEItemOperationsContext *context)
{
	int item_kind_no;
	int value;
	
	context->ExtractItemKindNo(item_kind_no);
	context->ExtractValue(value);
	
	warband->item_kinds[item_kind_no].swing_damage &= 0xFF;
	warband->item_kinds[item_kind_no].swing_damage |= value << 8;
}

void ItemSetHeadArmor(WSEItemOperationsContext *context)
{
	int item_kind_no;
	int value;
	
	context->ExtractItemKindNo(item_kind_no);
	context->ExtractValue(value);
	
	warband->item_kinds[item_kind_no].head_armor = value;
}

void ItemSetBodyArmor(WSEItemOperationsContext *context)
{
	int item_kind_no;
	int value;
	
	context->ExtractItemKindNo(item_kind_no);
	context->ExtractValue(value);
	
	warband->item_kinds[item_kind_no].body_armor = value;
}

void ItemSetLegArmor(WSEItemOperationsContext *context)
{
	int item_kind_no;
	int value;
	
	context->ExtractItemKindNo(item_kind_no);
	context->ExtractValue(value);
	
	warband->item_kinds[item_kind_no].leg_armor = value;
}

void ItemSetSpeedRating(WSEItemOperationsContext *context)
{
	int item_kind_no;
	int value;
	
	context->ExtractItemKindNo(item_kind_no);
	context->ExtractValue(value);
	
	warband->item_kinds[item_kind_no].speed_rating = value;
}

void ItemSetMissileSpeed(WSEItemOperationsContext *context)
{
	int item_kind_no;
	int value;
	
	context->ExtractItemKindNo(item_kind_no);
	context->ExtractValue(value);
	
	warband->item_kinds[item_kind_no].missile_speed = value;
}

WSEItemOperationsContext::WSEItemOperationsContext() : WSEOperationContext("item", 3800, 3899)
{
}

void WSEItemOperationsContext::OnLoad()
{
	RegisterOperation("item_get_weight", ItemGetWeight, Both, Lhs, 2, 2,
		"Stores <1>'s weight into <0>",
		"destination_fixed_point", "item_kind_no");

	RegisterOperation("item_get_value", ItemGetValue, Both, Lhs, 2, 2,
		"Stores <1>'s value into <0>",
		"destination", "item_kind_no");

	RegisterOperation("item_get_difficulty", ItemGetDifficulty, Both, Lhs, 2, 2,
		"Stores <1>'s difficulty into <0>",
		"destination", "item_kind_no");

	RegisterOperation("item_get_head_armor", ItemGetHeadArmor, Both, Lhs, 2, 2,
		"Stores <1>'s head armor into <0>",
		"destination", "item_kind_no");

	RegisterOperation("item_get_body_armor", ItemGetBodyArmor, Both, Lhs, 2, 2,
		"Stores <1>'s body armor into <0>",
		"destination", "item_kind_no");

	RegisterOperation("item_get_leg_armor", ItemGetLegArmor, Both, Lhs, 2, 2,
		"Stores <1>'s leg armor into <0>",
		"destination", "item_kind_no");

	RegisterOperation("item_get_hit_points", ItemGetHitPoints, Both, Lhs, 2, 2,
		"Stores <1>'s hit points into <0>",
		"destination", "item_kind_no");

	RegisterOperation("item_get_weapon_length", ItemGetWeaponLength, Both, Lhs, 2, 2,
		"Stores <1>'s weapon length into <0>",
		"destination", "item_kind_no");

	RegisterOperation("item_get_speed_rating", ItemGetSpeedRating, Both, Lhs, 2, 2,
		"Stores <1>'s speed rating into <0>",
		"destination", "item_kind_no");

	RegisterOperation("item_get_missile_speed", ItemGetMissileSpeed, Both, Lhs, 2, 2,
		"Stores <1>'s missile speed into <0>",
		"destination", "item_kind_no");

	RegisterOperation("item_get_max_ammo", ItemGetMaxAmmo, Both, Lhs, 2, 2,
		"Stores <1>'s max ammo into <0>",
		"destination", "item_kind_no");

	RegisterOperation("item_get_accuracy", ItemGetLegArmor, Both, Lhs, 2, 2,
		"Stores <1>'s accuracy into <0>",
		"destination", "item_kind_no");

	RegisterOperation("item_get_shield_width", ItemGetWeaponLength, Both, Lhs, 2, 2,
		"Stores <1>'s shield width into <0>",
		"destination", "item_kind_no");

	RegisterOperation("item_get_shield_height", ItemGetMissileSpeed, Both, Lhs, 2, 2,
		"Stores <1>'s shield height into <0>",
		"destination", "item_kind_no");

	RegisterOperation("item_get_horse_scale", ItemGetWeaponLength, Both, Lhs, 2, 2,
		"Stores <1>'s horse scale into <0>",
		"destination", "item_kind_no");

	RegisterOperation("item_get_horse_speed", ItemGetMissileSpeed, Both, Lhs, 2, 2,
		"Stores <1>'s horse speed into <0>",
		"destination", "item_kind_no");

	RegisterOperation("item_get_horse_maneuver", ItemGetSpeedRating, Both, Lhs, 2, 2,
		"Stores <1>'s horse maneuver into <0>",
		"destination", "item_kind_no");

	RegisterOperation("item_get_food_quality", ItemGetHeadArmor, Both, Lhs, 2, 2,
		"Stores <1>'s food quality into <0>",
		"destination", "item_kind_no");

	RegisterOperation("item_get_abundance", ItemGetAbundance, Both, Lhs, 2, 2,
		"Stores <1>'s abundance into <0>",
		"destination", "item_kind_no");

	RegisterOperation("item_get_thrust_damage", ItemGetThrustDamage, Both, Lhs, 2, 2,
		"Stores <1>'s thrust damage into <0>",
		"destination", "item_kind_no");

	RegisterOperation("item_get_thrust_damage_type", ItemGetThrustDamageType, Both, Lhs, 2, 2,
		"Stores <1>'s thrust damage type into <0>",
		"destination", "item_kind_no");

	RegisterOperation("item_get_swing_damage", ItemGetSwingDamage, Both, Lhs, 2, 2,
		"Stores <1>'s thrust damage into <0>",
		"destination", "item_kind_no");

	RegisterOperation("item_get_swing_damage_type", ItemGetSwingDamageType, Both, Lhs, 2, 2,
		"Stores <1>'s thrust damage type into <0>",
		"destination", "item_kind_no");

	RegisterOperation("item_get_horse_charge_damage", ItemGetThrustDamage, Both, Lhs, 2, 2,
		"Stores <1>'s thrust damage into <0>",
		"destination", "item_kind_no");

	RegisterOperation("item_has_property", ItemHasProperty, Both, Cf, 2, 2,
		"Fails if <item_kind_no> doesn't have <property>",
		"item_kind_no", "property");

	RegisterOperation("item_has_capability", ItemHasCapability, Both, Cf, 2, 2,
		"Fails if <0> doesn't have <1>",
		"item_kind_no", "capability");

	RegisterOperation("item_has_modifier", ItemHasModifier, Both, Cf, 2, 2,
		"Fails if <1> is not a valid modifier for <0>",
		"item_kind_no", "item_modifier_no");

	RegisterOperation("item_has_faction", ItemHasFaction, Both, Cf, 2, 2,
		"Fails if <0> doesn't have <1> in its faction list",
		"item_kind_no", "faction_no");

	RegisterOperation("item_set_thrust_damage", ItemSetThrustDamage, Both, None, 2, 2,
		"Sets <0>'s thrust damage to <1>",
		"item_kind_no", "value");

	RegisterOperation("item_set_thrust_damage_type", ItemSetThrustDamageType, Both, None, 2, 2,
		"Sets <0>'s thrust damage type to <1>",
		"item_kind_no", "value");

	RegisterOperation("item_set_swing_damage", ItemSetSwingDamage, Both, None, 2, 2,
		"Sets <0>'s thrust damage to <1>",
		"item_kind_no", "value");

	RegisterOperation("item_set_swing_damage_type", ItemSetSwingDamageType, Both, None, 2, 2,
		"Sets <0>'s thrust damage type to <1>",
		"item_kind_no", "value");

	RegisterOperation("item_set_head_armor", ItemSetHeadArmor, Both, None, 2, 2,
		"Sets <0>'s head armor to <1>",
		"item_kind_no", "value");

	RegisterOperation("item_set_body_armor", ItemSetBodyArmor, Both, None, 2, 2,
		"Sets <0>'s body armor to <1>",
		"item_kind_no", "value");

	RegisterOperation("item_set_leg_armor", ItemSetLegArmor, Both, None, 2, 2,
		"Sets <0>'s leg armor to <1>",
		"item_kind_no", "value");

	RegisterOperation("item_set_speed_rating", ItemSetSpeedRating, Both, None, 2, 2,
		"Sets <0>'s speed rating to <1>",
		"item_kind_no", "value");

	RegisterOperation("item_set_missile_speed", ItemSetMissileSpeed, Both, None, 2, 2,
		"Sets <0>'s missile speed to <1>",
		"item_kind_no", "value");
}
