#pragma once

#include "rgl.h"
#include "game_window.h"

namespace wb
{
	struct yes_no_window : game_window
	{
		rgl::popup_widget *popup_widget;
		rgl::widget *title_widget;
		rgl::widget *text_widget;
		rgl::text_box_widget *text_box_widget;
		rgl::widget *label_widget;
		rgl::combo_button_widget *combo_button_widget;
		rgl::game_button_widget *button_1_widget;
		rgl::game_button_widget *button_2_widget;
		rgl::simple_text_box_widget *simple_text_box_1_widget;
		rgl::simple_text_box_widget *simple_text_box_2_widget;
		int u1;
		int u2;
		int *output_result;
		rgl::string *output_text;
		rgl::string *output_simple_text_1;
		rgl::string *output_simple_text_2;
		int *output_combo_index;
		rgl::string textbox_text;
		rgl::string label_text;
		rgl::string *combo_texts;
		int num_combo_texts;
		bool show_cursor;
		rgl::string title;
		rgl::string button_1_text;
		rgl::string button_2_text;
	};
}
