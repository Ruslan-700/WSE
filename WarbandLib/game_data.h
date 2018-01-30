#pragma once

#include "d3dx9.h"
#include "rgl.h"
#include "action.h"
#include "config_manager.h"
#include "conversation.h"
#include "core_game.h"
#include "face_generator.h"
#include "flora_kind.h"
#include "game.h"
#include "game_keys.h"
#include "game_window.h"
#include "ground.h"
#include "input_manager.h"
#include "item_modifier.h"
#include "item_kind.h"
#include "language_manager.h"
#include "map_icon.h"
#include "menu.h"
#include "mesh.h"
#include "meeting_redirector.h"
#include "meta_mission.h"
#include "mission.h"
#include "mission_template.h"
#include "module_settings.h"
#include "multiplayer_data.h"
#include "music.h"
#include "network.h"
#include "particle_system.h"
#include "party_template.h"
#include "postfx.h"
#include "presentation.h"
#include "presentation_container.h"
#include "registry_manager.h"
#include "scene_prop.h"
#include "script.h"
#include "skill.h"
#include "skeleton.h"
#include "skybox.h"
#include "sound.h"
#include "steam.h"
#include "game_string.h"
#include "tableau.h"
#include "terrain_generator.h"

#pragma pack(push, 4)

namespace wb
{
	struct directx_call
	{
		int type;
		rgl::string string;
		int u1;
		void *object;
		int value;
	};
	
	struct game_data
	{
#if defined WARBAND_DEDICATED
		network_manager network_manager;
		int junk[556];
#endif
		rgl::io_text_stream debug_text_stream;
		int uuuuu1; // align
		rgl::io_text_stream debug_text_stream_snapshot;
		int uuuuu2; // align
		rgl::io_output_text_file_stream log_stream;
		int uuuuu3; // align
		config_manager config;
		int uuuuu4;
		config_manager options_config;
#if defined WARBAND_VANILLA
		int uuuuu5[55561];
#elif defined WARBAND_STEAM
		int uuuuu5[10331];
#elif defined WARBAND_DEDICATED
		int uuuuu5[9203];
#endif
		rgl::resource_manager resource_manager;
		bool error;
		int cur_frame_no;
		int rand_seed;
#if defined WARBAND
		//int uuuuu6;
		rgl::window_manager window_manager;
#endif
		//int uuuu1[48];
#if defined WARBAND
		//int uuuu1[40];
		int uuuu1[42];//1168
#elif defined WARBAND_DEDICATED
		//int uuuu1[41];
		int uuuu1[42];//1168
#endif
#if defined WARBAND
		int uuuu2;
		int uuuu3;
		network_manager network_manager;
#endif
		char path_buffer[256];
		int uuuu4;
		int uuu0; // align
		rgl::priority_selector priority_selector;
		int uuu1;
		//LPVOID mumble_shared_memory;
		//HANDLE mumble_handle;
		int uuu2; // align
		rgl::string uuu3[6];
		int uuu4; // TODO: agent ai related
		int uuu5; // TODO: agent ai related
		int uuu6; // TODO: agent ai related
		rgl::vector4 uuu7; // TODO: agent ai related
#if defined WARBAND_DEDICATED
		int uuu7_1; // align
		double cur_cos;
		double cur_sin;
#endif
		rgl::body_part *blocking_body_part;
#if defined WARBAND
		bool in_party_window;
#endif
		//unsigned int bits_4;
		//unsigned int bits_2;
#if defined WARBAND
		int uuu8; // align
		rgl::matrix mesh_shading_matrix;
		unsigned int mesh_shading_matrix_initialized;
#endif
		int uuu9[3];
		int cur_visitor_site_no;
		int cur_statement_no;
		int script_error_occurred;
		int cur_opcode;
		int skip_next_operation;
#if defined WARBAND
		/*
		int uu0[3]; // align
		bool main_window_focused;
		bool dof_enabled;
		bool hdr_enabled;
		bool uu1;
		int uu2[133];
		*/
		int uu0[2];// align
		int uu3[7];
		bool uu1;
		bool main_window_focused;
		bool dof_enabled;
		bool hdr_enabled;
		int uu2[133];
		//DWORD update_check_thread_id;
		HANDLE rendering_event_1;
		HANDLE rendering_event_2;
		HANDLE rendering_event_3;
		//CRITICAL_SECTION rendering_critical_section;
		int uu6;
		int uu7;
		HANDLE multithreaded_texture_loader_event;
		//CRITICAL_SECTION multithreaded_texture_creator_critical_section;
		//CRITICAL_SECTION multithreaded_texture_loader_critical_section;
		int hlsl_mode;
#elif defined WARBAND_DEDICATED
		//int uu0[128];
		int uu0[113];
#endif
		int display_wp[7];
#if defined WARBAND
		meeting_redirector meeting_redirector;
#endif
		mission *cur_mission;
		game *cur_game;
#if defined WARBAND
		//CRITICAL_SECTION multithreading_critical_section;
		//CRITICAL_SECTION directx_critical_section;
		int uu14;//1166
		rgl::font *default_font;
		int uu15;//1166
#endif
		int unknown_structure[128];//1166
#if defined WARBAND_DEDICATED
		int uu16;//1166
		int uu15;//1166
#endif
		bool reset_3d_env;
		bool quitting;
		bool u2;
#if defined WARBAND
		ID3DXEffect *effect;
		rgl::effect_state_manager *effect_state_manager;
		int uu11[2]; // unused
		int uu17;//1166
#else if defined WARBAND_DEDICATED
		//int uu11[3]; // unused
		int uu11[2]; // unused
#endif
		__int64 timers[NUM_TIMERS];
		__int64 diff_timers[NUM_TIMERS];
#if defined WARBAND
		//int uu1211; // align
		presentation_container *cur_presentation;
		int uu12; // align
#else if defined WARBAND_DEDICATED
		int uu17;//1166
#endif
		int uu13[2]; // unused
		menu *menus;
		int num_menus;
		int compatible_module_version;
		int compatible_savegame_module_version;
		int savegame_version;
		int module_version;
#if defined WARBAND_DEDICATED //1168
		int full_version;
#endif	
		int give_performance_warnings;
		int dont_suppress_initial_warnings;
#if defined WARBAND	
		int disable_moveable_flag_optimization;
		int u4[3];//1170
#else if defined WARBAND_DEDICATED
		int u4[2];//1170
#endif		
		//int u4[2];
		item_kind *item_kinds;
		int num_item_kinds;
		int u5; // align
		scene_prop *scene_props;
		int num_scene_props;
		int u6; // align
#if defined WARBAND
		post_effect *post_effects;
		int num_post_effects;
#endif
		flora_kind *flora_kinds;
		int num_flora_kinds;
		skybox *skyboxes;
		int num_skyboxes;
		map_icon *map_icons;
		int num_map_icons;
		party_template *party_templates;
		int num_party_templates;
		mission_template *mission_templates;
		int num_mission_templates;
#if defined WARBAND
		DWORD directx_thread_id;
#endif
		DWORD main_thread_id;
#if defined WARBAND
		int intro_video_stage;
		int reset_device;
		int display_global_message;
		presentation_manager presentation_manager;
#else if defined WARBAND_DEDICATED
		int u7; // align
#endif
		language_manager language_manager;
		registry_manager registry_manager;
		input_manager input_manager;
#if defined WARBAND
		//rgl::renderer renderer;
		int renderer[405175];//1166 temp
		rgl::graphics_context graphics_context;
#endif
		skeleton_manager skeleton_manager;
		module_settings module_settings;
#if defined WARBAND
		particle_system_manager particle_system_manager;
#else if defined WARBAND_DEDICATED
		int u17;//1173 align
#endif
		rgl::physics_manager physics_manager;
#if defined WARBAND
		HANDLE game_started_mutex;
#else if defined WARBAND_DEDICATED
		unsigned int bbbb;
#endif
		rgl::ini_file resources_ini_file;
		int u9; // align
		rgl::ini_file compatibility_resources_ini_file;
		int u10; // align
		rgl::ini_file game_variables_ini_file;
		int u11; // align
#if defined WARBAND
		rgl::shadowmap_renderer shadowmap_renderer;
		//int u12; // align
#endif
		rgl::string module_names[32]; //1166
		basic_game basic_game;
		meta_mission meta_mission;
		game_key_manager game_key_manager;
#if defined WARBAND
		music_manager music_manager;
		game_screen game_screen;
		tableau_manager tableau_manager;
#endif
		string_manager string_manager;
		script_manager script_manager;
		multiplayer_data multiplayer_data;
		item_modifier item_modifiers[NUM_ITEM_MODIFIERS];
		action_set action_manager;
		sound_manager sound_manager;
#if defined WARBAND
		post_effect cur_post_effect;
#endif
		//terrain_generator terrain_generator;
#if defined WARBAND
		int terrain_generator[4118275];//1166 temp
#else if defined WARBAND_DEDICATED
		int terrain_generator[4083262];//1166 temp
#endif
#if defined WARBAND
		int newpadding; // align
#endif
		ground_spec ground_specs[NUM_GROUND_SPECS];
		mesh_manager mesh_manager;
		face_generator face_generator;
		conversation_manager conversation_manager;
		int u15; // align
		skill skills[NUM_SKILLS];
#if defined WARBAND_STEAM
		steam_manager steam_manager;
		steam_achievement_manager steam_achievement_manager;
		int u16;
#endif
		rgl::string cur_module_path;
		rgl::string cur_module_name;
		rgl::string cur_module_args;
		stl::vector<directx_call> directx_calls;
		//core_game core_game;
	};
}

#pragma pack(pop)
