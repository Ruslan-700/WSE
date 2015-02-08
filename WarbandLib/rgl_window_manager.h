#pragma once

#include <Windows.h>
#include "fmod.h"
#include "rgl_camera.h"
#include "rgl_mesh.h"
#include "rgl_meta_mesh.h"
#include "rgl_popup_widget.h"
#include "rgl_render_context.h"
#include "rgl_string.h"
#include "rgl_timer.h"
#include "rgl_vector2.h"
#include "rgl_widget.h"
#include "stl_vector.h"

namespace rgl
{
#if defined WARBAND
	struct message_queue
	{
		int size;
		int head;
		string texts[32];
		unsigned int colors[32];
	};

	struct window_manager
	{
		stl::vector<popup_widget *> popup_widgets;
		widget *focused_widget;
		vector2 cursor_position;
		int mouse_x;
		int mouse_y;
		int total_mouse_x;
		int total_mouse_y;
		vector2 prev_cursor_position;
		POINT prev_cursor_position_raw;
		int u1[2];
		render_context widget_render_context;
		render_context overlay_render_context;
		render_context tooltip_render_context;
		camera window_camera;
		camera tooltip_camera;
		float multiple_screen_mode_aspect_ratio;
		timer left_mouse_down_timer;
		timer right_mouse_down_timer;
		timer middle_mouse_down_timer;
		int window_size_x;
		int window_size_y;
		float vertical_aspect_ratio;
		float horizontal_aspect_ratio;
		bool show_cursor;
		bool u2;
		meta_mesh *cursor_overlay_meta_mesh;
		float u3;
		mesh *tooltip_mesh;
		timer tooltip_timer;
		mesh *context_menu_mesh;
		mesh context_menu_background_mesh;
		string tooltip_texts[80];
		bool tooltip_centered[80];
		float tooltip_sizes[80];
		unsigned int tooltip_colors[80];
		int tooltip_cur_line;
		float tooltip_max_width;
		float tooltip_left_offset;
		mesh frame_mesh;
		mesh *frame_corner_meshes[4];
		mesh tutorial_message_background_mesh;
		mesh tutorial_message_mesh;
		float tutorial_message_position_x;
		float tutorial_message_position_y;
		float tutorial_message_size_x;
		float tutorial_message_size_y;
		bool tutorial_message_center_justify;
		timer tutorial_message_timer;
		bool tutorial_message_is_timed;
		unsigned int tooltip_foreground_color;
		unsigned int tooltip_background_color;
		bool tooltip_open;
		vector2 tooltip_position;
		float tooltip_scale;
		int u4;
		vector2 context_menu_position;
		float tooltip_cur_y;
		float context_menu_cur_y;
		bool popup_closing;
		bool close_popup;
		FMOD_SOUND *fmod_click_sound;
		bool aspect_ratio_control_enabled;
		bool force_multiple_screen_mode;
		bool force_standard_aspect_ratio;
		bool multiple_screen_scrolling;
		bool typing;
		int scroll_amount;
		int pressed_key;
		float message_render_queue_cur_y;
		int message_render_queue_head;
		meta_mesh message_render_queue_meta_meshes[32];
		mesh message_render_queue_meshes[32];
		float message_render_queue_transparencies[32];
		float message_render_queue_y_positions[32];
		float aspect_ratio;
		unsigned int message_render_queue_colors[32];
		bool clear_depth_buffer;
		bool clear_render_target;
		unsigned int render_target_background_color;
		POINT cursor_position_raw;
		message_queue message_queue;
		stl::vector<string> message_log;
		int ping_samples[25];
		int cur_ping_sample;
		bool show_ping_samples;
		mesh ping_samples_mesh;
		int u6;//1166
		string tutorial_message_cur_text;
		unsigned int tutorial_message_cur_color;
		vector2 tutorial_message_cur_size;
		
		void set_tooltip_mesh(mesh *mesh, bool clear_texts);
		void set_show_cursor(bool value);
		void display_message(const string &text, const unsigned int &color, const bool &unknown);
	};
#endif
}
