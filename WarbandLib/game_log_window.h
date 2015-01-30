#pragma once

#include "rgl.h"
#include "game_window.h"

namespace wb
{
	struct game_log_window : game_window
	{
		rgl::popup_widget *popup_widget;
		rgl::hyperlink_text_widget *text_widget;
		rgl::board_widget *board_widget;
		rgl::scrollable_widget *scrollable_widget;
		rgl::simple_button_widget *return_button_widget;
		rgl::simple_button_widget *game_log_button_widget;
		rgl::simple_button_widget *recent_messages_button_widget;
		rgl::simple_button_widget *troops_button_widget;
		rgl::simple_button_widget *locations_button_widget;
		rgl::simple_button_widget *factions_button_widget;
		rgl::simple_button_widget *quests_button_widget;
		rgl::simple_button_widget *info_pages_button_widget;
	};
}
