#pragma once

#include "rgl.h"
#include "game_window.h"

namespace wb
{
	struct map_window : game_window
	{
		rgl::popup_widget *popup_widget;
		rgl::scene_widget *scene_widget;
		rgl::board_widget *bar_board_widget;
		rgl::simple_button_widget *reports_button_widget;
		rgl::simple_button_widget *party_button_widget;
		rgl::simple_button_widget *inventory_button_widget;
		rgl::simple_button_widget *character_button_widget;
		rgl::simple_button_widget *terrain_editor_button_widget;
		rgl::simple_button_widget *camp_button_widget;
		rgl::simple_button_widget *quests_button_widget;
		rgl::widget *paused_label_widget;
		rgl::widget *resume_label_widget;
		rgl::widget *time_of_day_widget;
		rgl::widget *date_widget;
		bool trial_ended;
		rgl::simple_text_box_widget *command_line_textbox_widget;
		rgl::widget *command_line_label_widget;
		rgl::string command_line_text;
		rgl::string time_of_day;
		int cur_hour;
		int cur_map_window_size_x;
		int cur_map_window_size_y;
		rgl::camera cam;
		rgl::vector4 camera_origin;
		rgl::vector4 camera_velocity;
		float camera_horizontal_rotation;
		float camera_vertical_rotation;
		float camera_zoom;
		rgl::vector2 camera_movement_speed;
		float camera_rotation_speed;
		float camera_zoom_speed;
		float selection_tooltip_time;
		float context_menu_time;
		bool mouse_moved;
		int context_menu_mode;
		int context_menu_object;
		bool refresh_cursor;
	};
}
