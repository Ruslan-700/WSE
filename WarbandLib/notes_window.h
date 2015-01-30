#pragma once

#include "rgl.h"
#include "game_window.h"
#include "presentation_container.h"

namespace wb
{
	struct notes_window : game_window
	{
		rgl::popup_widget *popup_widget;
		rgl::widget *picture_widget;
		rgl::hyperlink_text_widget *keys_hyperlink_widget;
		rgl::widget *title_widget;
		rgl::hyperlink_text_widget *data_hyperlink_widget;
		rgl::board_widget *keys_board_widget;
		rgl::board_widget *data_board_widget;
		rgl::scrollable_widget *keys_scrollable_widget;
		rgl::scrollable_widget *data_scrollable_widget;
		rgl::simple_button_widget *game_log_button_widget;
		rgl::simple_button_widget *recent_messages_button_widget;
		rgl::simple_button_widget *troops_button_widget;
		rgl::simple_button_widget *quests_button_widget;
		rgl::simple_button_widget *info_pages_button_widget;
		rgl::simple_button_widget *factions_button_widget;
		rgl::simple_button_widget *locations_button_widget;
		rgl::simple_button_widget *return_button_widget;
		rgl::simple_button_widget *back_button_widget;
		rgl::simple_button_widget *forward_button_widget;
		rgl::simple_button_widget *follow_button_widget;
		stl::vector<int> u1;
		stl::vector<int> u2;
		int u3[7];
	};
}
