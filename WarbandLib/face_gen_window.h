#pragma once

#include "rgl.h"
#include "agent.h"
#include "face_generator.h"
#include "game_window.h"

namespace wb
{
	struct face_gen_window : game_window
	{
		rgl::popup_widget *popup_widget;
		rgl::scene_widget *scene_widget;
		rgl::camera camera;
		float horizontal_rotation;
		float vertical_rotation;
		float zoom;
		bool rotating;
		bool zooming;
		int skin_no;
		rgl::scene *scene;
		rgl::scrollable_widget *deform_scrollable_widget;
		rgl::board_widget *deform_board_widget;
		rgl::board_widget *buttons_board_widget;
		rgl::simple_button_widget *face_keys_edit_mode_display_widget;
		rgl::slider_widget *face_key_slider_widgets[48];
		rgl::widget *face_key_label_widgets[48];
		rgl::simple_button_widget *next_beard_button_widget;
		rgl::simple_button_widget *next_hair_button_widget;
		rgl::simple_button_widget *next_skin_button_widget;
		rgl::simple_button_widget *prev_beard_button_widget;
		rgl::simple_button_widget *prev_hair_button_widget;
		rgl::simple_button_widget *prev_skin_button_widget;
		rgl::widget *change_beard_label_widget;
		rgl::widget *change_hair_label_widget;
		rgl::widget *change_skin_label_widget;
		rgl::slider_widget *age_slider_widget;
		rgl::slider_widget *hair_color_slider_widget;
		rgl::widget *change_age_label_widget;
		rgl::widget *change_hair_color_label_widget;
		rgl::simple_button_widget *done_button_widget;
		rgl::simple_button_widget *reset_button_widget;
		rgl::simple_button_widget *randomize_button_widget;
		int beard_no;
		int hair_no;
		int hair_texture_no;
		int face_texture_no;
		float age;
		float hair_color;
		face_keys face_keys_1;
		face_keys face_keys_2;
		rgl::meta_mesh *agent_meta_meshes[NUM_AGENT_META_MESHES];
		rgl::strategic_entity *character_entity;
		rgl::strategic_entity *terrain_entity;
		char face_key_1_text[40];
		char face_key_2_text[40];
		char face_key_3_text[40];
		char face_key_4_text[40];
	};
}
