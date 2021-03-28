#pragma once

#include "rgl.h"
#include "basics.h"
#include "simple_trigger.h"

namespace wb
{
	enum item_property : unsigned __int64
	{
		itp_force_attach_left_hand = 0x100,
		itp_force_attach_right_hand = 0x200,
		itp_force_attach_left_forearm = 0x300,
		itp_attach_armature = 0xF00,
		itp_attachment_mask = 0xF00,
		itp_unique = 0x1000,
		itp_always_loot = 0x2000,
		itp_no_parry = 0x4000,
		itp_default_ammo = 0x8000,
		itp_merchandise = 0x10000,
		itp_wooden_attack = 0x20000,
		itp_wooden_parry = 0x40000,
		itp_food = 0x80000,
		itp_cant_reload_on_horseback = 0x100000,
		itp_two_handed = 0x200000,
		itp_primary = 0x400000,
		itp_replaces_helm = 0x400000,
		itp_secondary = 0x800000, // DEPRECATED
		itp_replaces_shoes = 0x800000,
		itp_covers_legs = 0x1000000,
		itp_doesnt_cover_hair = 0x1000000,
		itp_can_penetrate_shield = 0x1000000,
		itp_consumable = 0x2000000,
		itp_bonus_against_shield = 0x4000000,
		itp_penalty_with_shield = 0x8000000,
		itp_cant_use_on_horseback = 0x10000000,
		itp_civilian = 0x20000000,
		itp_next_item_as_melee = 0x20000000,
		itp_offset_lance = 0x40000000,
		itp_covers_head = 0x80000000,
		itp_couchable = 0x80000000,
		itp_crush_through = 0x100000000,
		itp_remove_item_on_use = 0x400000000,
		itp_unbalanced = 0x800000000,
		itp_covers_beard = 0x1000000000,
		itp_no_pick_up_from_ground = 0x2000000000,
		itp_can_knock_down = 0x4000000000,
		itp_covers_hair = 0x8000000000,
		itp_force_show_body = 0x10000000000,
		itp_force_show_left_hand = 0x20000000000,
		itp_force_show_right_hand = 0x40000000000,
		itp_covers_hair_partially = 0x80000000000,
		itp_extra_penetration = 0x100000000000,
		itp_has_bayonet = 0x200000000000,
		itp_cant_reload_while_moving = 0x400000000000,
		itp_ignore_gravity = 0x800000000000,
		itp_ignore_friction = 0x1000000000000,
		itp_is_pike = 0x2000000000000,
		itp_offset_musket = 0x4000000000000,
		itp_no_blur = 0x8000000000000,
		itp_cant_reload_while_moving_mounted = 0x10000000000000,
		itp_has_upper_stab = 0x20000000000000,
		itp_disable_agent_sounds = 0x40000000000000,
		itp_is_carriage = 0x80000000000000,

		itp_kill_info_mask = 0x700000000000000,
		itp_kill_info_bits = 56,

		itp_shield_no_parry = 0x4000, //WSE
		itp_offset_mortschlag = 0x1000000000000000, //WSE
		itp_covers_hands = 0x1000000000000000, //WSE
		itp_offset_flip = 0x4000000000000000, //WSE
	};

	enum item_capability : unsigned __int64
	{
		itcf_thrust_onehanded = 0x1,
		itcf_overswing_onehanded = 0x2,
		itcf_slashright_onehanded = 0x4,
		itcf_slashleft_onehanded = 0x8,
		itcf_thrust_twohanded = 0x10,
		itcf_overswing_twohanded = 0x20,
		itcf_slashright_twohanded = 0x40,
		itcf_slashleft_twohanded = 0x80,
		itcf_thrust_polearm = 0x100,
		itcf_overswing_polearm = 0x200,
		itcf_slashright_polearm = 0x400,
		itcf_slashleft_polearm = 0x800,
		itcf_shoot_bow = 0x1000,
		itcf_shoot_crossbow = 0x4000,
		itcf_throw_stone = 0x10000,
		itcf_throw_knife = 0x20000,
		itcf_throw_axe = 0x30000,
		itcf_throw_javelin = 0x40000,
		itcf_shoot_pistol = 0x70000,
		itcf_shoot_musket = 0x80000,
		itcf_shoot_mask = 0xFF000,
		itcf_horseback_slashright_onehanded = 0x800000,
		itcf_horseback_slashleft_onehanded = 0x1000000,
		itcf_thrust_onehanded_lance = 0x4000000,
		itcf_thrust_onehanded_lance_horseback = 0x8000000,
		itcf_carry_mask = 0x7F0000000,
		itcf_carry_sword_left_hip = 0x10000000,
		itcf_carry_axe_left_hip = 0x20000000,
		itcf_carry_dagger_front_left = 0x30000000,
		itcf_carry_dagger_front_right = 0x40000000,
		itcf_carry_quiver_front_right = 0x50000000,
		itcf_carry_quiver_back_right = 0x60000000,
		itcf_carry_quiver_right_vertical = 0x70000000,
		itcf_carry_quiver_back = 0x80000000,
		itcf_carry_revolver_right = 0x90000000,
		itcf_carry_pistol_front_left = 0xA0000000,
		itcf_carry_bowcase_left = 0xB0000000,
		itcf_carry_mace_left_hip = 0xC0000000,
		itcf_carry_axe_back = 0x100000000,
		itcf_carry_sword_back = 0x110000000,
		itcf_carry_kite_shield = 0x120000000,
		itcf_carry_round_shield = 0x130000000,
		itcf_carry_buckler_left = 0x140000000,
		itcf_carry_crossbow_back = 0x150000000,
		itcf_carry_bow_back = 0x160000000,
		itcf_carry_spear = 0x170000000,
		itcf_carry_board_shield = 0x180000000,
		itcf_carry_katana = 0x210000000,
		itcf_carry_wakizashi = 0x220000000,
		itcf_show_holster_when_drawn = 0x800000000,
		itcf_reload_pistol = 0x7000000000,
		itcf_reload_musket = 0x8000000000,
		itcf_reload_mask = 0xF000000000,
		itcf_parry_forward_onehanded = 0x10000000000,
		itcf_parry_up_onehanded = 0x20000000000,
		itcf_parry_right_onehanded = 0x40000000000,
		itcf_parry_left_onehanded = 0x80000000000,
		itcf_parry_forward_twohanded = 0x100000000000,
		itcf_parry_up_twohanded = 0x200000000000,
		itcf_parry_right_twohanded = 0x400000000000,
		itcf_parry_left_twohanded = 0x800000000000,
		itcf_parry_forward_polearm = 0x1000000000000,
		itcf_parry_up_polearm = 0x2000000000000,
		itcf_parry_right_polearm = 0x4000000000000,
		itcf_parry_left_polearm = 0x8000000000000,
		itcf_horseback_slash_polearm = 0x10000000000000,
		itcf_overswing_spear = 0x20000000000000,
		itcf_overswing_musket = 0x40000000000000,
		itcf_thrust_musket = 0x80000000000000,
	};

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
