#pragma once

#include "rgl.h"
#include "operation.h"

namespace wb
{
	enum trigger_interval
	{
		ti_simulate_battle                    = -5,
		ti_on_party_encounter                 = -6,
		ti_question_answered                  = -8,
		ti_server_player_joined               = -15,
		ti_on_multiplayer_mission_end         = -16,
		ti_before_mission_start               = -19,
		ti_after_mission_start                = -20,
		ti_tab_pressed                        = -21,
		ti_inventory_key_pressed              = -22,
		ti_escape_pressed                     = -23,
		ti_battle_window_opened               = -24,
		ti_on_agent_spawn                     = -25,
		ti_on_agent_killed_or_wounded         = -26,
		ti_on_agent_knocked_down              = -27,
		ti_on_agent_hit                       = -28,
		ti_on_player_exit                     = -29,
		ti_on_leave_area                      = -30,
		ti_on_scene_prop_init                 = -40,
		ti_on_scene_prop_hit                  = -42,
		ti_on_scene_prop_destroy              = -43,
		ti_on_scene_prop_use                  = -44,
		ti_on_scene_prop_is_animating         = -45,
		ti_on_scene_prop_animation_finished   = -46,
		ti_on_scene_prop_start_use            = -47,
		ti_on_scene_prop_cancel_use           = -48,
		ti_on_init_item                       = -50,
		ti_on_weapon_attack                   = -51,
		ti_on_missile_hit                     = -52,
		ti_on_item_picked_up                  = -53,
		ti_on_item_dropped                    = -54,
		ti_on_agent_mount                     = -55,
		ti_on_agent_dismount                  = -56,
		ti_on_item_wielded                    = -57,
		ti_on_item_unwielded                  = -58,
		ti_on_presentation_load               = -60,
		ti_on_presentation_run                = -61,
		ti_on_presentation_event_state_change = -62,
		ti_on_presentation_mouse_enter_leave  = -63,
		ti_on_presentation_mouse_press        = -64,
		ti_on_init_map_icon                   = -70,
		ti_on_order_issued                    = -71,
		ti_on_switch_to_map                   = -75,
		ti_scene_prop_deformation_finished    = -76,
		ti_on_shield_hit                      = -80,
		ti_on_scene_prop_stepped_on           = -100, //WSE
		ti_on_init_missile                    = -101, //WSE
		ti_on_agent_turn                      = -102, //WSE
		ti_on_agent_blocked                   = -103, //WSE
		ti_on_missile_dive                    = -104, //WSE
		ti_on_agent_start_reloading           = -105, //WSE
		ti_on_agent_end_reloading             = -106, //WSE
		ti_on_shield_penetrated               = -107, //WSE
		ti_on_scene_prop_is_deforming         = -108, //WSE
		ti_once                               = 100000000,
	};

	enum trigger_status
	{
		ts_ready = 0,
		ts_delay = 1,
		ts_rearm = 2,
	};

	struct trigger
	{
		float check_interval;
		float delay_interval;
		float rearm_interval;
		trigger_status status;
		operation_manager conditions;
		operation_manager consequences;
		rgl::timer check_interval_timer;
		rgl::timer delay_interval_timer;
		rgl::timer rearm_interval_timer;

		void execute(int context);
	};

	struct trigger_manager
	{
		int num_triggers;
		trigger *triggers;
		int timer_no;

		void execute(int context);
		int addTrigger(const trigger &newTrigger);
		bool removeTrigger(int index);
	};
}
