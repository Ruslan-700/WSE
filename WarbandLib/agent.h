#pragma once

#include "fmod.h"
#include "rgl.h"
#include "action.h"
#include "agent_ai.h"
#include "basics.h"
#include "item.h"
#include "missile.h"
#include "mission_grid.h"
#include "simple_game_object.h"
#include "Physics\Collide\Query\Collector\PointCollector\hkpAllCdPointCollector.h"
#include "Physics\Dynamics\Entity\hkpRigidBody.h"
#include "Physics\Utilities\CharacterControl\CharacterProxy\hkpCharacterProxy.h"

#define NUM_AGENT_META_MESHES 28

namespace wb
{
	enum agent_horse_walk_state
	{
		hws_walk_backwards = 0x0,
		hws_stand = 0x1,
		hws_walk = 0x2,
		hws_trot = 0x3,
		hws_canter = 0x4,
		hws_gallop = 0x5,
		hws_pace_mask = 0x7,
		hws_accelerating = 0x10,
		hws_decelerating = 0x20,
		hws_movement_mask = 0x30,
	};

	enum agent_body_meta_mesh
	{
		bmm_head = 0,
		bmm_beard = 1,
		bmm_hair = 2,
		bmm_helmet = 3,
		bmm_armor = 4,
		bmm_trousers = 5,
		bmm_left_foot = 6,
		bmm_right_foot = 7,
		bmm_armature = 8,
		bmm_item_1 = 9,
		bmm_item_2 = 10,
		bmm_item_3 = 11,
		bmm_item_4 = 12,
		bmm_missile_1 = 13,
		bmm_missile_2 = 14,
		bmm_missile_3 = 15,
		bmm_missile_4 = 16,
		bmm_carry_1 = 17,
		bmm_carry_2 = 18,
		bmm_carry_3 = 19,
		bmm_carry_4 = 20,
		bmm_unknown_2 = 21,
		bmm_left_hand = 22,
		bmm_right_hand = 23,
		bmm_left_bracer = 24,
		bmm_right_bracer = 25,
		bmm_banner = 26,
		bmm_name = 27,
	};

	enum agent_flags
	{
		af_player = 0x1,
	};

	enum agent_status
	{
		as_alive   = 1,
		as_wounded = 4,
		as_dead    = 5,
	};

	enum agent_type
	{
		at_human = 0,
		at_horse = 1,
	};

	enum agent_controller
	{
		ac_other = 0,
		ac_bot   = 1,
		ac_human = 2,
	};

	enum agent_blow_flags
	{
		abf_couch       = 0x10,
		abf_silent      = 0x20,
		abf_crushtrough = 0x40,
	};
	
	struct agent_blow
	{
		unsigned int flags;
		int agent_no;
		rgl::vector4 position;
		rgl::vector4 direction;
		rgl::vector4 rotation;
		int hit_bone_no;
		int rear_horse;
		float raw_damage;
		float inflicted_damage;
		int soaked_damage;
		int damage_type;
		item item;
		missile *missile;
		bool ranged;
		bool strong_strike;
		bool no_interrupt;
		rgl::vector4 missile_rotation;
		rgl::vector4 missile_position;
		int weapon_class;
	};

	struct agent_action_channel
	{
		int action_no;
		int next_action_no;
		int sequence_no;
		int u1;
		int u2;
		unsigned int master_flags;
		unsigned int next_master_flags;
		float progress;
		float duration;
		float speed_factor;
		float target_progress;
		int length;
	};

	struct agent;

	struct agent_cd_point_collector : hkpAllCdPointCollector
	{
		bool is_bot;
		bool is_horse;
		agent *agent;
	};

	struct agent : simple_game_object
	{
		rgl::vector4 movement_dir;
		float movement_dirf;
		int mission_object_in_use;
		agent_type type;
		int gender_no;
		agent_status status;
		unsigned int flags;
		int player_no;
		bool fire_ranged;
		rgl::vector4 position;
		rgl::vector4 u1;
		int u2;
		int approximate_position_x;
		int approximate_position_y;
		int approximate_position_z;
		float movement_dirf_change;
		float look_dirf_change;
		float u3;
		float u4;
		float u5;
		rgl::matrix scaled_frame;
		rgl::matrix rot_frame;
		slot_manager slots;
		int horse_agent_no;
		int rider_agent_no;
		int spawn_rider_agent_no;
		int stand_action_no;
		int walk_forward_action_no;
		unsigned int movement_flags;
		rgl::timer movement_timer;
		int last_hit_agent_no;
		stl::vector<int> offer_agent_nos;
		stl::vector<float> offer_timeouts;
		bool has_relations;
		stl::vector<int> relation_agent_nos;
		stl::vector<int> relation_values;
		rgl::timer mission_object_use_timer;
		attack attack_rec;
		rgl::timer defend_timer;
		rgl::timer attack_timer;
		rgl::timer attack_cancel_timer;
		float u6;
		float u7;
		agent_controller controller;
		int strafe_direction;
		rgl::timer strafe_timer;
		float scale;
		float lance_offset;
		float death_bobbing_modifier;
		float rotation_damping;
		int troop_no;
		item horse_item;
		float horse_skew;
		unsigned int dna;
		rgl::vector4 collision_movement;
		rgl::vector2 speed;
		rgl::vector2 max_speed;
		int u8;
		int walk_state;
		bool sprinting_in_prev_frame;
		bool sprinting_in_cur_frame;
		bool yell;
		rgl::timer yell_timer;
		rgl::timer sprint_timer;
		rgl::timer stun_timer;
		rgl::timer keep_start_timer;
		rgl::timer keep_timer;
		int u11;
		float couched_lance_time;
		int couched_lance_mode;
		rgl::quad *ground_quad;
		float rotation_speed;
		int u12;
		rgl::vector4 last_ai_mesh_check_position;
		agent_cd_point_collector cast_point_collector;
		agent_cd_point_collector start_point_collector;
		hkpCharacterProxy *havok_physics;
		agent_action_channel action_channels[2];
		action_set *action_set;
		bool u13;
		unsigned int wielded_items_flags;
		int u14;
		int u15;
#if defined WARBAND
		rgl::meta_mesh *body_meta_meshes[NUM_AGENT_META_MESHES];
#endif
		bool show_banner;
		rgl::particle_system *hoof_dust_particle_system;
		rgl::particle_system *water_splash_1_particle_system;
		rgl::particle_system *water_splash_2_particle_system;
		rgl::particle_system *water_splash_3_particle_system;
		int party_no;
		int party_stack_no;
		int team_no;
		int group_no;
		int division_no;
		rgl::timer division_update_timer;
		int division_member_no;
		item items[NUM_ITEM_SLOTS];
		bool items_not_picked_up[NUM_ITEM_SLOTS];
		int wielded_items[2];
		int wielded_equipped_items[2];
		int both_hands_occupied;
		int item_alternative_usages[4];
		int armor[3];
		float total_item_weight;
		float horse_speed_factor;
		rgl::strategic_entity *entity;
		agent_ai ai;
		int attack_action_direction;
		int attack_action_type;
		int defend_action_direction;
		float defend_action_duration;
		rgl::timer defend_action_timer;
		int num_agents_looking_at;
		int ready_animation_no;
		int release_animation_no;
		int attack_action;
		int defend_action;
		int initiate_defend_action;
		rgl::timer attack_ready_timer;
		bool no_ammo_message_displayed;
		int defend_animation_no;
		int keep_animation_no;
		int queued_defend_animation_no;
		int queued_keep_animation_no;
		int queued_ready_animation_no;
		int queued_release_animation_no;
		int queued_attack_direction;
		float defend_action_charge;
		int kick_status;
		bool kick_allowed;
		int queued_reload_animation_no;
		rgl::vector4 stored_look_dir;
		float accuracy_related_1;
		float accuracy_related_2;
		rgl::vector4 prediction_prev_position;
		rgl::timer prediction_timer;
		rgl::vector4 predicted_position;
		rgl::vector4 predicted_velocity;
		int cur_blow_status;
		agent_blow cur_blow;
		bool touched;
		int hit_bone_no;
		rgl::vector4 hit_force;
		float modifiers[6];
		rgl::vector4 acceleration;
		rgl::vector4 velocity;
		rgl::vector4 direction;
		float accuracy_movement_modifier;
		int rider_rotation_lock_no;
		float rider_rotation;
		bool u16;
		float u17;
		float slope;
		float u18;
		int weapon_usage_order;
		float cur_action_progress_for_sound;
		float prev_action_progress_for_sound;
		float horse_turn_speed;
		int horse_turn_state;
		int acceleration_state;
		int strafe_state;
		unsigned int alter_flags;
		int item_overrides[10];
		int num_item_overrides;
		int entry_point_no;
		bool dynamics;
		float hit_points_at_spawn;
		float hit_points_max;
		float hit_points_float;
		int hit_points_int;
		bool no_death_knockdown_only;
		bool invulnerable_shield;
		int kill_count;
		int wounded_count;
		int horse_charge_agent_no;
		bool horse_charge;
		float horse_charge_time;
		int experience;
		float u20[2];
		bool routed;
		rgl::toggle_timer dismount_timer;
		rgl::timer exit_timer;
		rgl::timer ranged_hold_timer;
		rgl::timer jump_kick_cooldown_timer;
		rgl::timer horse_snort_timer;
		rgl::timer stuck_timer;
		rgl::timer death_timer;
		rgl::timer horse_charged_horse_timer;
		rgl::timer couch_parried_timer;
		rgl::timer horse_turn_timer;
		int horse_turn_direction;
		int exit_status;
		bool has_corpse;
		int horse_agent_before_death_no;
		hkpRigidBody *horse_agent_before_death_body;
		float u21;
		int running_away_state;
		int sound_channel;
		int module_sound_channel;
#if defined WARBAND
		int reloading_sound_channel;
#endif
		int num_times_cheered;
		bool update_banner;
		rgl::ai_mesh_face *ai_mesh_face;
		int has_ragdoll;
		int ragdoll_hit_bone_no;
		rgl::vector4 ragdoll_hit_force;
		float ragdoll_hit_duration;
		//rgl::matrix ragdoll_position;
		bool ragdoll_update_position;
		bool ragdoll_set_bone_velocity;
		rgl::vector4 ragdoll_velocity;
		bool ragdoll_fell_on_ground;
		int ragdoll_world_no;
		rgl::vector4 ragdoll_starting_position;
		rgl::timer yell_cooldown_timer;
		rgl::timer ragdoll_timer;
		float frame_time;
		int u22;
		int attached_scene_prop_no;
		float attached_scene_prop_position_x;
		float attached_scene_prop_position_y;
		float attached_scene_prop_position_z;
		float u23;
		float u24;
		float u25;
		float rider_rot_min;
		float rider_rot_max;
		mission_grid_location grid_location;
		unsigned int prev_actions_2;
		agent_control control;
		//float u26;
		//float u27;
		int u28; //1166
		rgl::timer u29; //1166
		float u30; //1166
		int u31; //1166

		rgl::string get_name() const;
		item &get_wielded_item(item &item, int hand);
		void set_first_person_body_meta_meshes_visibility(unsigned int visibility_flags);
		int get_relation_with(agent &agent) const;
		bool is_ally_with(agent *agent) const;
		bool is_enemy_with(agent *agent) const;
		rgl::entity *get_ground_entity() const;
		bool is_couching() const;
		void set_ai_behavior(int behavior);
		void add_stun(float duration);
		void play_sound_at_position(bool *networkBroadcast, int type, bool *altSoundForPlayer, int *soundNo, int *playerSoundNo, const rgl::vector4 &position, float *frequency);
	};
}
