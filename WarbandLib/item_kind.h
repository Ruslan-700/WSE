#pragma once

#include "rgl.h"
#include "basics.h"
#include "simple_trigger.h"

namespace wb
{
	enum item_kind_type
	{
		itp_type_horse      = 1,
		itp_type_one_handed = 2,
		itp_type_two_handed = 3,
		itp_type_polearm    = 4,
		itp_type_arrows     = 5,
		itp_type_bolts      = 6,
		itp_type_shield     = 7,
		itp_type_bow        = 8,
		itp_type_crossbow   = 9,
		itp_type_thrown     = 10,
		itp_type_goods      = 11,
		itp_type_head_armor = 12,
		itp_type_body_armor = 13,
		itp_type_foot_armor = 14,
		itp_type_hand_armor = 15,
		itp_type_pistol     = 16,
		itp_type_musket     = 17,
		itp_type_bullets    = 18,
		itp_type_animal     = 19,
		itp_type_book       = 20,
	};

	struct item_kind_variation
	{
		rgl::string id;
		unsigned __int64 flags;
	};

	struct item_kind
	{
		rgl::string name;
		rgl::string mesh_name;
		rgl::string id;
		int faction_nos[16];
		int num_faction_nos;
		slot_manager slots;
		int u1;
		int num_variations;
#if defined WARBAND
		rgl::meta_mesh *variation_meta_meshes[16];
#endif
		item_kind_variation variations[16];
		unsigned __int64 properties;
		unsigned __int64 capabilities;
		int value;
		unsigned __int64 modifiers;
		int hit_points;
		int difficulty;
		int speed_rating;
		int missile_speed;
		int head_armor;
		int body_armor;
		int leg_armor;
		float weight;
		int weapon_length;
		int max_ammo;
		int thrust_damage;
		int swing_damage;
		float price_rate;
		int abundance;
		float probability;
		simple_trigger_manager simple_triggers;

		item_kind_type get_type() const;
		bool is_weapon() const;
		bool is_melee_weapon() const;
		bool is_ranged_weapon() const;
		bool is_shield() const;
		bool is_horse() const;
		bool is_animal() const;
		bool is_ammo() const;
		int get_attachment_bone_no() const;
	};
}
