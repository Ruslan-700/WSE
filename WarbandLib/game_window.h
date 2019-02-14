#pragma once

#include "rgl.h"

#define NUM_GAME_WINDOWS 32

namespace wb
{
	enum game_window_type
	{
		gwt_yes_no = 0,
		gwt_selection = 1,
		gwt_starting = 2,
		gwt_loading = 3,
		gwt_initial = 4,
		gwt_escape = 5,
		gwt_map = 6,
		gwt_inventory = 7,
		gwt_party = 8,
		gwt_quests = 9,
		gwt_tactical = 10,
		gwt_character = 11,
		gwt_conversation = 12,
		gwt_options = 13,
		gwt_graphics_options = 14,
		gwt_controls = 15,
		gwt_load_save = 16,
		gwt_terrain_gen = 17,
		gwt_menu = 18,
		gwt_stats = 19,
		gwt_game_log = 20,
		gwt_notes = 21,
		gwt_presentation = 22,
		gwt_credits = 23,
		gwt_quit_ads = 24,
		gwt_profile = 25,
		gwt_multiplayer_client = 26,
		gwt_multiplayer_server = 27,
		gwt_banner_selection = 28,
		gwt_quick_battle = 29,
		gwt_group = 30,
		gwt_face_gen = 31,
	};

	struct game_window
	{
		int type;
		int u1;
		int u2;
		int u3;
		int object;
		int source_window_no;
		rgl::string u4;
		rgl::string u5;

		virtual ~game_window() = 0;
		virtual void close() = 0;
		virtual void lose_focus() = 0;
		virtual void open() = 0;
		virtual void obtainFocus() = 0;
		virtual void run(float time) = 0;
		virtual void reset() = 0;
		virtual void save_state(const rgl::io_stream &stream) = 0;
		virtual void load_state(const rgl::io_stream &stream) = 0;
		virtual bool is_fullscreen() = 0;
		virtual bool has_cursor() = 0;
	};

	struct game_screen
	{
		game_window *game_windows[NUM_GAME_WINDOWS];
		stl::deque<int> open_windows;
	};
}
