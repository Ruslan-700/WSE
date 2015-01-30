#pragma once

#include "rgl_font.h"
#include "rgl_mesh.h"
#include "rgl_render_context.h"
#include "rgl_string.h"
#include "rgl_timer.h"
#include "rgl_vector2.h"
#include "stl_vector.h"

#define NUM_WIDGET_MESHES 10

namespace rgl
{
	enum widget_mesh_kind
	{
		wmk_background       = 0,
		wmk_border_left      = 1,
		wmk_border_right     = 2,
		wmk_button_down      = 3,
		wmk_button_up        = 4,
		wmk_button_highlight = 5,
		wmk_text             = 6,
		wmk_highlight        = 7,
		wmk_cursor           = 8,
		wmk_structure        = 9,
	};

	enum widget_type
	{
		wt_base = 0,
		wt_simple_button = 1,
		wt_slider = 2,
		wt_fill_slider = 3,
		wt_image = 4,
		wt_combo_button = 5,
		wt_text_box = 6,
		wt_simple_text_box = 7,
		wt_scrollable = 8,
		wt_image_button = 9,
		wt_game_button = 10,
		wt_number_box = 11,
		wt_list_box = 12,
		wt_table = 13,
		wt_combo_label = 14,
		wt_scrollbar = 15,
	};

	enum widget_flags
	{
		wf_hidden   = 0x1,
		wf_disabled = 0x2,
		wf_autosize = 0x100,
	};

	enum widget_state_flags
	{
		wsf_mouse_inside = 0x1,
		wsf_mouse_enter = 0x2,
		wsf_mouse_leave = 0x4,
		wsf_mouse_left_up = 0x8,
		wsf_mouse_left_down = 0x10,
		wsf_mouse_right_up = 0x20,
		wsf_mouse_right_down = 0x40,
		wsf_mouse_middle_up = 0x80,
		wsf_mouse_middle_down = 0x100,
		wsf_mouse_left_staying_down = 0x200,
		wsf_mouse_right_staying_down = 0x400,
		wsf_mouse_middle_staying_down = 0x800,
		wsf_key_press = 0x1000,
		wsf_click = 0x10000,
		wsf_close = 0x20000,
		wsf_double_click = 0x40000,
	};

	struct widget
	{
		timer click_timer;
		timer animate_to_color_timer;
		timer animate_to_alpha_timer;
		timer animate_to_highlight_color_timer;
		timer animate_to_highlight_alpha_timer;
		timer animate_to_size_timer;
		timer animate_to_position_timer;
		float animate_to_color_duration;
		float animate_to_alpha_duration;
		float animate_to_highlight_color_duration;
		float animate_to_highlight_alpha_duration;
		float animate_to_size_duration;
		float animate_to_position_duration;
		vector2 animate_to_position_target;
		vector2 animate_to_size_target;
		unsigned int animate_to_color_target;
		unsigned int animate_to_alpha_target;
		unsigned int animate_to_highlight_color_target;
		unsigned int animate_to_highlight_alpha_target;
		vector2 animate_to_position_base;
		vector2 animate_to_size_base;
		unsigned int animate_to_color_base;
		unsigned int animate_to_alpha_base;
		unsigned int animate_to_highlight_color_base;
		unsigned int animate_to_highlight_alpha_base;
		bool is_animating_to_color;
		bool is_animating_to_alpha;
		bool is_animating_to_highlight_color;
		bool is_animating_to_highlight_alpha;
		bool is_animating_to_size;
		bool is_animating_to_position;
		unsigned int state_flags;
		unsigned int trigger_state_flags;
		unsigned int trigger_state_mask;
		unsigned int trigger_flags;
		string tooltip_text;
		int tooltip_state;
		timer mouse_enter_timer;
		bool vertex_animated;
		vector2 size;
		vector2 absolute_position;
		vector2 relative_position;
		vector2 upper_right_offset;
		vector2 lower_left_offset;
		unsigned int flags;
		mesh *meshes[NUM_WIDGET_MESHES];
		bool meshes_in_use[NUM_WIDGET_MESHES];
		bool highlighted;
		bool dynamic;
		font *text_font;
		string text;
		vector2 text_position;
		vector2 text_size;
		bool text_is_password;
		unsigned int text_flags;
		unsigned int text_color;
		unsigned int text_highlight_color;
		widget *container_widget;
		widget *parent_widget;
		int value_i;
		int value_i_min;
		int value_i_max;
		float value_f;
		float value_f_min;
		float value_f_max;
		float z_offset;

		virtual ~widget();
		virtual void set_tooltip_text_2(const string &text);
		virtual bool is_highlightable() const;
		virtual void check_tooltip();
		virtual void show_tooltip();
		virtual void set_tooltip_text(const string &text);
		virtual bool set_use_password_char(const bool &value);
		virtual void run(float time);
		virtual void autosize();
		virtual void animate_to_color(float duration, unsigned int color);
		virtual void animate_to_alpha(float duration, unsigned int color);
		virtual void animate_to_highlight_color(float duration, unsigned int color);
		virtual void animate_to_highlight_alpha(float duration, unsigned int color);
		virtual void animate_to_size(float duration, const vector2 &size);
		virtual void animate_to_position(float duration, const vector2 &position);
		virtual void render(render_context *render_context, float &alpha, const vector2 &lower_left, const vector2 &upper_right);
		virtual void set_value_i(int value);
		virtual int get_value_i() const;
		virtual void set_value_f(float value);
		virtual float get_value_f() const;
		virtual bool has_state(unsigned int state) const;
		virtual void clear_state();
		virtual bool check_state(unsigned int state);
		virtual bool set_state(unsigned int state);
		virtual bool set_trigger_mask(unsigned int mask);
		virtual bool set_trigger_flags(unsigned int flags);
		virtual bool set_trigger_flag(unsigned int flag);
		virtual bool update_child_position(widget *child);
		virtual bool update_position();
		virtual void set_upper_right_offset(const vector2 &pos);
		virtual void set_lower_left_offset(const vector2 &pos);
		virtual void set_text_size(const vector2 &size);
		virtual void set_text_position(const vector2 &position);
		virtual void set_text_flags(const unsigned int &flags);
		virtual void set_text_flag(const unsigned int &flag);
		virtual void set_mesh_fit_to_positions(int index, mesh *mesh, const vector2 &min_pos, const vector2 &max_pos);
		virtual void set_mesh_fit_to_size(int index, mesh *mesh);
		virtual void set_mesh(int index, mesh *mesh);
		virtual void set_mesh_tiled(int index, mesh *mesh, const vector2 &lower_left, const vector2 &upper_right);
		virtual void set_mesh_vertical(int index, const string &top_mesh_name, const string &mid_mesh_name, const string &bottom_mesh_name, const vector2 &lower_left, const vector2 &upper_right);
		virtual void set_mesh_horizontal(int index, const string &top_mesh_name, const string &mid_mesh_name, const string &bottom_mesh_name, const vector2 &lower_left, const vector2 &upper_right);
		virtual void set_mesh_frame(int index, const string &horizontal_mesh_name, const string &vertical_mesh_name, const vector2 &lower_left, const float &upper_right, const float &thickness);
		virtual bool change_state(unsigned int state);
		virtual void update_state();
		virtual void lose_focus();
		virtual void obtain_focus();
		virtual vector2 &redraw_text(vector2 &);
		virtual void set_text_color(const unsigned int &color);
		virtual void set_text_highlight_color(const unsigned int &color);
		virtual void set_font(font *font);
		virtual vector2 set_text(const string &text);
		virtual void get_text(const string &text) const;
		virtual double measure_text();
		virtual bool draw_text(const string &text, const vector2 &position);
		virtual void hide();
		virtual void show();
		virtual void disable();
		virtual void enable();
		virtual widget_type get_type() const;
		widget();
	};
}
