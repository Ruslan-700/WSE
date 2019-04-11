#pragma once

#include "rgl.h"
#include "faction.h"
#include "info_page.h"
#include "map_event.h"
#include "map_track.h"
#include "party.h"
#include "quest.h"
#include "simple_trigger.h"
#include "site.h"
#include "trigger.h"
#include "troop.h"

namespace wb
{
	struct party_stack_dna
	{
		unsigned int dnas[32];
	};

	struct window_redirect
	{
		int redirect_no;
		int presentation_no;
	};

	struct game
	{
		int savegame_slot;
		int seed;
		rgl::string savegame_name;
		int u1;
		bool u2;
		rgl::string module_name;
		rgl::mesh *track_mesh;
		rgl::strategic_entity *track_entity;
		stl::vector<rgl::mesh *> map_tree_meshes;
		rgl::mesh *skybox_mesh;
		rgl::vector4 sun_color;
		rgl::vector4 fog_color;
		rgl::vector4 ambient_color;
		int scene_hdr_skybox_no;
		int scene_skybox_no;
		float scene_fog_distance;
		unsigned int scene_fog_color;
		rgl::vector4 scene_sun_color;
		rgl::vector4 scene_ambient_color;
		rgl::vector4 scene_ground_ambient_color;
		rgl::string class_names[9];
		int main_party_no;
		int temp_party_no;
		int camp_bandits_party_no;
		int camera_follow_party_no;
		bool camera_follow_party_enabled;
		int player_troop_no;
		int temp_troop_no;
		bool paused;
		bool spotting_range_cheat;
		int player_agent_kill_count;
		int player_agent_own_troop_kill_count;
		int player_agent_wounded_count;
		int player_agent_own_troop_wounded_count;
		bool mission_object_in_use;
		unsigned int encounter_flags_1;
		int background_presentation_no;
		unsigned int encounter_flags_2;
		int saving_allowed;
		int ironman;
		rgl::string cur_menu_text;
		stl::vector<window_redirect> window_redirects;
		rgl::stable_vector<map_event> map_events;
		rgl::hash_vector<party> parties;
		rgl::light *sun_light;
#if defined WARBAND
		int u3;
#endif
		int camera_mode;
		trigger_manager triggers;
		simple_trigger_manager simple_triggers;
		troop *troops;
		int num_troops;
		int u4;
		int encountered_party_1_no;
		int encountered_party_2_no;
		int encountered_party_is_attacker;
		bool meta_mission_is_active;
		int repeat_object;
		int default_prisoner_price;
		int u5;
		int u6;
		int u7;
		int mercenary_source_party_no;
		int started;
		int merchandise_modifier_quality;
		bool map_conversation_start;
		int map_conversation_token;
		int map_conversation_troop_1_no;
		int map_conversation_troop_2_no;
		faction *factions;
		int num_factions;
		site *sites;
		int num_sites;
		bool additional_scene_added;
		quest *quests;
		int num_quests;
		info_page *info_pages;
		int num_info_pages;
		stl::vector<map_track> tracks;
		rgl::string enable_main_party_dialog_title;
		rgl::timer enable_main_party_timer;
		rgl::timer last_disembark_check_timer;
		bool enable_main_party_check;
		int main_party_action_type;
		rgl::string dialog_box_title;
		rgl::string dialog_box_text;
		bool dialog_box_show;
		float map_legend_zoom;
		float rest_period;
		int rest_remain_attackable;
		int rest_is_interactive;
		float rest_time_speed;
		int party_creation_min_random_value;
		int party_creation_max_random_value;
		int displayed_tutorials;
		int cur_menu_no;
		int cur_screen_troop_no;
		int cur_scene_no;
		int cur_mission_entry_no;
		int cur_mission_template_no;
		int cur_screen_party_no;
		int cur_screen_note_type;
		int cur_screen_note_object_no;
		bool mission_enable_talk;
		bool mission_disable_talk;
		rgl::timer date;
		int hour;
		int day;
		int week;
		int month;
		int year;
		float global_cloud_amount;
		float global_haze_amount;
		float average_difficulty;
		float difficulty_period;
		float last_difficulty_calculation;
		rgl::string log;
		int u8;
		float u9[24];
		int context_menu_num_items;
		int context_menu_mouseover_index;
		rgl::string context_menu_items[50];
		int context_menu_values[50];
		rgl::vector4 cur_color;
		rgl::vector4 cur_local_position;
		rgl::matrix u10;
		rgl::matrix cur_position;
		bool disable_menu_option;
		float time_multiplier;
		party_stack_dna main_party_dnas[32];
		party_stack_dna attacking_party_dnas[32];
		party_stack_dna defending_party_dnas[32];
		bool game_active;
		int script_debug_mode_enabled;
		int script_debug_mode_dialog_visibile;
		int script_debug_mode_currently_debugging;
		rgl::string script_debug_mode_script_name;
		int script_debug_mode_line;
		rgl::string script_debug_mode_file_contents;
		int console_mode;
		int edit_mode_active;
		int edit_mode_mode;
		int edit_mode_add_objects_mode;
		rgl::vector4 edit_mode_scale;
		float edit_mode_altitude;
		float edit_mode_camera_movement_speed;
		int edit_mode_camera_movement_speed_time;
		int edit_mode_cur_meta_type;
		int edit_mode_cur_sub_kind_no;
		int edit_mode_prev_meta_type;
		int edit_mode_prev_sub_kind_no;
		bool edit_mode_move_to_object;
		int edit_mode_randomize_rotation;
		int edit_mode_randomize_scale;
		float edit_mode_ground_elevation_brush_radius;
		float edit_mode_ground_leveling_brush_radius;
		float edit_mode_ground_paint_brush_radius;
		float edit_mode_ground_elevation_brush_weight;
		float edit_mode_ground_leveling_brush_weight;
		float edit_mode_ground_paint_brush_weight;
		float edit_mode_ground_elevation_brush_hardness;
		float edit_mode_ground_leveling_brush_hardness;
		float edit_mode_ground_paint_brush_hardness;
		rgl::vector4 edit_mode_ground_paint_color;
		int edit_mode_ground_elevation_mode;
		float edit_mode_ground_leveling_height;
		rgl::orientation u11;
		rgl::matrix edit_mode_camera_movement_target_position;
		rgl::timer edit_mode_camera_movement_timer;
		int edit_mode_dialog_open;
		rgl::matrix edit_mode_camera_movement_cur_position;
		rgl::vector4 edit_mode_ai_mesh_face_placement_position;
		int edit_mode_highlighted_mission_object_no;
		stl::vector<int> edit_mode_selected_mission_object_nos;
		bool edit_mode_selection_changed;
		int edit_mode_changes_made;
		int edit_mode_ai_mesh_selection_type;
		int u12[6];
		int trigger_mission_object_no;
		int trigger_agent_no;
		int trigger_troop_no;
#if defined WARBAND
		rgl::meta_mesh *trigger_meta_mesh;
		rgl::mesh *trigger_mesh;
#endif
		int trigger_item_slot_no;
		int question_answer;
		int hide_tactical_ui;
		int show_object_details_overlay;
		rgl::string background_mesh_name;
		int game_menu_tableau_material_no;
		int game_menu_tableau_material_value;
		float game_menu_tableau_material_position[4];
		int talk_info_relation_bar_value;
		int talk_info_show;
		rgl::string talk_info_lines[5];
		bool talk_info_in_use;
		rgl::timer last_food_check;
		rgl::strategic_entity *compass_entity;

		bool execute_script(int id, int param_1 = 0, int param_2 = 0, int param_3 = 0, int param_4 = 0, int param_5 = 0, int param_6 = 0) const;
	};
}
