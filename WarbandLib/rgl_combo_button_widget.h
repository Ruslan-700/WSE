#pragma once

#include "rgl_button_widget.h"
#include "rgl_popup_widget.h"
#include "rgl_widget.h"
#include "rgl_string.h"
#include "rgl_vector2.h"
#include "stl_vector.h"

namespace rgl
{
	struct combo_button_widget : button_widget
	{
		bool u1;
		widget *popup_widget;
		stl::vector<string> texts;
		widget *widgets[10];
		vector2 child_button_size;
		float child_button_position;
		vector2 child_button_text_size;
		vector2 child_button_text_position;
		
		virtual void clear_popup();
		virtual void add_widget(int index, widget *widget);
		virtual void clear_texts();
		virtual void remove_text(const int &index);
		virtual void insert_text(const string &text, const int &index);
		virtual void add_text(const string &text);
	};
}
