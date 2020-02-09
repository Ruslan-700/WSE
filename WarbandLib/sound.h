#pragma once

#include "fmod.h"
#include "rgl.h"

#define NUM_MAPPED_SOUNDS 130

namespace wb
{
	enum mapped_sounds
	{
		snd_click,
		snd_gong,
		snd_quest_taken,
		snd_quest_completed,
		snd_quest_succeeded,
		snd_quest_concluded,
		snd_quest_failed,
		snd_quest_cancelled,
		snd_rain,
		snd_money_paid,
		snd_money_received,
		snd_sword_clash_1,
		snd_sword_clash_2,
		snd_sword_clash_3,
		snd_sword_swing,
		snd_footstep_grass,
		snd_footstep_wood,
		snd_footstep_water,
		snd_footstep_horse,
		snd_footstep_horse_1b,
		snd_footstep_horse_1f,
		snd_footstep_horse_2b,
		snd_footstep_horse_2f,
		snd_footstep_horse_3b,
		snd_footstep_horse_3f,
		snd_footstep_horse_4b,
		snd_footstep_horse_4f,
		snd_footstep_horse_5b,
		snd_footstep_horse_5f,
		snd_jump_begin,
		snd_jump_end,
		snd_jump_begin_water,
		snd_jump_end_water,
		snd_horse_jump_begin,
		snd_horse_jump_end,
		snd_horse_jump_begin_water,
		snd_horse_jump_end_water,
		snd_release_bow,
		snd_release_crossbow,
		snd_throw_javelin,
		snd_throw_axe,
		snd_throw_knife,
		snd_throw_stone,
		snd_reload_crossbow,
		snd_reload_crossbow_continue,
		snd_pull_bow,
		snd_pull_arrow,
		snd_arrow_pass_by,
		snd_bolt_pass_by,
		snd_javelin_pass_by,
		snd_stone_pass_by,
		snd_axe_pass_by,
		snd_knife_pass_by,
		snd_bullet_pass_by,
		snd_incoming_arrow_hit_ground,
		snd_incoming_bolt_hit_ground,
		snd_incoming_javelin_hit_ground,
		snd_incoming_stone_hit_ground,
		snd_incoming_axe_hit_ground,
		snd_incoming_knife_hit_ground,
		snd_incoming_bullet_hit_ground,
		snd_outgoing_arrow_hit_ground,
		snd_outgoing_bolt_hit_ground,
		snd_outgoing_javelin_hit_ground,
		snd_outgoing_stone_hit_ground,
		snd_outgoing_axe_hit_ground,
		snd_outgoing_knife_hit_ground,
		snd_outgoing_bullet_hit_ground,
		snd_draw_sword,
		snd_put_back_sword,
		snd_draw_greatsword,
		snd_put_back_greatsword,
		snd_draw_axe,
		snd_put_back_axe,
		snd_draw_greataxe,
		snd_put_back_greataxe,
		snd_draw_spear,
		snd_put_back_spear,
		snd_draw_crossbow,
		snd_put_back_crossbow,
		snd_draw_revolver,
		snd_put_back_revolver,
		snd_draw_dagger,
		snd_put_back_dagger,
		snd_draw_bow,
		snd_put_back_bow,
		snd_draw_shield,
		snd_put_back_shield,
		snd_draw_other,
		snd_put_back_other,
		snd_body_fall_small,
		snd_body_fall_big,
		snd_horse_body_fall_begin,
		snd_horse_body_fall_end,
		snd_shield_broken,
		snd_hide,
		snd_unhide,
		snd_neigh,
		snd_gallop,
		snd_battle,
		snd_arrow_hit_body,
		snd_blunt_hit,
		snd_player_hit_by_arrow,
		snd_horse_walk,
		snd_horse_trot,
		snd_horse_canter,
		snd_horse_gallop,
		snd_horse_breath,
		snd_horse_snort,
		snd_block_fist,
		snd_hit_wood_wood,
		snd_hit_metal_metal,
		snd_hit_wood_metal,
		snd_shield_hit_wood_wood,
		snd_shield_hit_metal_metal,
		snd_shield_hit_wood_metal,
		snd_shield_hit_metal_wood,
		snd_metal_hit_low_armor_low_damage,
		snd_metal_hit_low_armor_high_damage,
		snd_metal_hit_high_armor_low_damage,
		snd_metal_hit_high_armor_high_damage,
		snd_wooden_hit_low_armor_low_damage,
		snd_wooden_hit_low_armor_high_damage,
		snd_wooden_hit_high_armor_low_damage,
		snd_wooden_hit_high_armor_high_damage,
		snd_mp_arrow_hit_target,
		snd_release_crossbow_medium,
		snd_release_crossbow_far,
		snd_bullet_hit_body,
		snd_player_hit_by_bullet,
	};

	enum sound_flags
	{
		sf_2d                  = 0x1,
		sf_looping             = 0x2,
		sf_start_at_random_pos = 0x4,
		sf_priority_mask       = 0xF0,
		sf_volume_mask         = 0xF00,
	};

	struct sound
	{
		rgl::string id;
		unsigned int flags;
		int sample_nos[32];
		unsigned int sample_faction_nos[32];
		int num_samples;
	};

	struct sound_sample
	{
		FMOD_SOUND *fmod_sound;
		unsigned int flags;
		float length;
		float frequency;
		int pcm_samples;
	};

	struct sound_manager
	{
		int num_sounds;
		sound *sounds;
#if defined WARBAND_DEDICATED
		int u1[18];//1166
#endif		
		int mapped_sounds[NUM_MAPPED_SOUNDS];
		float sound_distances[NUM_MAPPED_SOUNDS];
	};
}
