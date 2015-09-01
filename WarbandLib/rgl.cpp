#include "rgl.h"

#include <cassert>

using namespace rgl;

void rgl::check_types()
{
	assert_size<matrix>(64, 64);
	assert_size<string>(144, 144);
	assert_size<timer>(16, 16);
	assert_size<vector2>(8, 8);
	assert_size<vector4>(16, 16);

	assert_size<ai_mesh>(140, 140);
	assert_size<camera>(296, -1);
	assert_size<effect_state_manager>(84, -1);
	assert_size<font>(14480, -1);
	//assert_size<graphics_context>(1036, -1);
	assert_size<graphics_context>(1244, -1);
	assert_size<ini_file>(1179652, 1179652);
	assert_size<light>(124, -1);
	assert_size<manifold>(104, 104);
	assert_size<bounding_box>(52, -1);
	//assert_size<meta_mesh>(352, -1);
	assert_size<meta_mesh>(372, -1);
	assert_size<particle>(100, -1);
	assert_size<particle_system>(444, -1);
	assert_size<physics_manager>(140, 140);
	assert_size<postfx_node>(188, -1);
	assert_size<priority_selector>(240008, 240008);
	//assert_size<renderer>(18792312, -1);
	assert_size<renderer>(18792584, -1);
	//assert_size<render_context>(684, -1);
	assert_size<render_context>(688, -1);
	assert_size<render_mesh>(2368, -1);
	assert_size<render_params>(556, -1);
	//assert_size<resource_manager>(1700, 1636);
	assert_size<resource_manager>(1708, 1676);
	assert_size<scene>(884, 276);
	//assert_size<shadowmap_renderer>(708, -1);
	assert_size<shadowmap_renderer>(712, -1);
	assert_size<terrain>(32, 32);
#if defined WARBAND
	//assert_size<window_manager>(64472, -1);
	assert_size<window_manager>(65416, -1);
#endif

	assert_size<io_stream>(308, 308);
	assert_size<io_binary_stream>(308, 308);
	assert_size<io_text_stream>(308, 308);
	assert_size<io_input_binary_file_stream>(316, 316);
	assert_size<io_input_text_file_stream>(316, 316);
	assert_size<io_output_binary_file_stream>(316, 316);
	assert_size<io_output_text_file_stream>(316, 316);
	assert_size<input_binary_file_stream>(316, 316);
	assert_size<input_text_file_stream>(316, 316);
	assert_size<output_binary_file_stream>(316, 316);
	assert_size<output_binary_file_stream_auto_flush>(328, 328);
	assert_size<output_text_file_stream>(316, 316);

	assert_size<resource>(160, 160);
	assert_size<body>(164, 164);
	assert_size<hlsl_technique>(192, -1);
	assert_size<material>(372, 332);
	assert_size<mesh>(888, -1);
	assert_size<shader>(848, 848);
	assert_size<skeleton_anim>(192, 192);
	assert_size<skeleton_model>(192, 192);
	assert_size<skeleton_model_bone>(408, 408);
	assert_size<skeleton_model_body>(48, 48);
	assert_size<texture>(248, -1);

	assert_size<body_part>(68, 68);
	assert_size<capsule>(136, 136);
	assert_size<composite_body>(76, 76);
	assert_size<manifold_body>(116, 116);
	assert_size<quad>(284, 284);
	assert_size<sphere>(104, 104);

	assert_size<bone>(540, 536);
	assert_size<channel>(200, 200);
	assert_size<skeleton>(144, 144);

	assert_size<entity>(340, 244);
	assert_size<mesh_entity>(340, 244);
	assert_size<static_entity>(340, 244);
	assert_size<strategic_entity>(728, 260);

	assert_size<widget>(720, -1);
	assert_size<button_widget>(728, -1);
	assert_size<container_widget>(888, -1);
	assert_size<game_button_widget>(744, -1);
	assert_size<image_button_widget>(736, -1);
	assert_size<popup_widget>(896, -1);
	assert_size<simple_button_widget>(736, -1);
	assert_size<text_box_widget>(768, -1);
	assert_size<simple_text_box_widget>(768, -1);
	assert_size<combo_button_widget>(824, -1);
	assert_size<scene_widget>(1416, -1);
	assert_size<list_box_widget>(992, -1);
	assert_size<progress_bar_widget>(752, -1);
	assert_size<board_widget>(888, -1);
	assert_size<hyperlink_text_widget>(808, -1);
	//assert_size<scrollable_widget>(1904, -1);
	assert_size<scrollable_widget>(1912, -1);
	assert_size<scrollbar_widget>(1608, -1);
	assert_size<number_box_widget>(928, -1);
	assert_size<combo_label_widget>(920, -1);
	assert_size<check_box_widget>(904, -1);
	assert_size<slider_widget>(736, -1);
}
