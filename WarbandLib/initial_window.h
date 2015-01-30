#pragma once

#include "rgl.h"
#include "game_window.h"
#include "presentation_container.h"

namespace wb
{
	struct initial_window : game_window
	{
		rgl::popup_widget *popup_widget;
		presentation_container presentation_container;
		rgl::simple_button_widget *tutorial_button_widget;
		rgl::simple_button_widget *start_game_button_widget;
		rgl::simple_button_widget *multiplayer_button_widget;
		rgl::simple_button_widget *resume_game_button_widget;
		rgl::simple_button_widget *custom_battle_button_widget;
		rgl::simple_button_widget *options_button_widget;
		rgl::simple_button_widget *credits_button_widget;
		rgl::simple_button_widget *quit_button_widget;
		rgl::widget *version_info_text_widget;
		rgl::widget *game_logo_image_widget;
		int first_free_save_game;
	};
}
