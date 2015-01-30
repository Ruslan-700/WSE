#pragma once

#include "rgl_common.h"
#include "rgl_addresses.h"
#include "rgl_functions.h"

#include "rgl_quaternion.h"
#include "rgl_matrix.h"
#include "rgl_string.h"
#include "rgl_timer.h"
#include "rgl_vector2.h"
#include "rgl_vector4.h"

#include "rgl_ai_mesh.h"
#include "rgl_bounding_box.h"
#include "rgl_camera.h"
#include "rgl_effect_state_manager.h"
#include "rgl_font.h"
#include "rgl_graphics_context.h"
#include "rgl_ini_file.h"
#include "rgl_light.h"
#include "rgl_manifold.h"
#include "rgl_meta_mesh.h"
#include "rgl_particle_system.h"
#include "rgl_physics.h"
#include "rgl_postfx_node.h"
#include "rgl_priority_selector.h"
#include "rgl_renderer.h"
#include "rgl_render_context.h"
#include "rgl_render_mesh.h"
#include "rgl_render_params.h"
#include "rgl_resource_manager.h"
#include "rgl_scene.h"
#include "rgl_shadowmap.h"
#include "rgl_terrain.h"
#include "rgl_window_manager.h"

#include "rgl_pool.h"
#include "rgl_recycled_vector.h"
#include "rgl_stable_vector.h"
#include "rgl_static_deque.h"
#include "rgl_hash_vector.h"
#include "stl_deque.h"
#include "stl_list.h"
#include "stl_priority_queue.h"
#include "stl_vector.h"
#include "stl_vector_bool.h"

#include "rgl_io_stream.h"
#include "rgl_io_input_binary_file_stream.h"
#include "rgl_io_input_text_file_stream.h"
#include "rgl_io_output_binary_file_stream.h"
#include "rgl_io_output_text_file_stream.h"
#include "rgl_input_binary_file_stream.h"
#include "rgl_input_text_file_stream.h"
#include "rgl_output_binary_file_stream.h"
#include "rgl_output_binary_file_stream_auto_flush.h"
#include "rgl_output_text_file_stream.h"

#include "rgl_resource.h"
#include "rgl_body.h"
#include "rgl_hlsl_technique.h"
#include "rgl_material.h"
#include "rgl_mesh.h"
#include "rgl_shader.h"
#include "rgl_skeleton_anim.h"
#include "rgl_skeleton_model.h"
#include "rgl_texture.h"

#include "rgl_body_part.h"
#include "rgl_capsule.h"
#include "rgl_composite_body.h"
#include "rgl_manifold_body.h"
#include "rgl_quad.h"
#include "rgl_sphere.h"

#include "rgl_bone.h"
#include "rgl_channel.h"
#include "rgl_skeleton.h"
#include "rgl_ipo.h"

#include "rgl_entity.h"
#include "rgl_mesh_entity.h"
#include "rgl_static_entity.h"
#include "rgl_strategic_entity.h"

#include "rgl_widget.h"
#include "rgl_button_widget.h"
#include "rgl_container_widget.h"
#include "rgl_game_button_widget.h"
#include "rgl_image_button_widget.h"
#include "rgl_popup_widget.h"
#include "rgl_simple_button_widget.h"
#include "rgl_text_box_widget.h"
#include "rgl_simple_text_box_widget.h"
#include "rgl_combo_button_widget.h"
#include "rgl_scene_widget.h"
#include "rgl_list_box_widget.h"
#include "rgl_progress_bar_widget.h"
#include "rgl_board_widget.h"
#include "rgl_hyperlink_text_widget.h"
#include "rgl_scrollable_widget.h"
#include "rgl_scrollbar_widget.h"
#include "rgl_number_box_widget.h"
#include "rgl_combo_label_widget.h"
#include "rgl_check_box_widget.h"
#include "rgl_slider_widget.h"

#define UNREACHABLE DebugBreak()

namespace rgl
{
	void check_types();
}

template <typename T>
void assert_size(unsigned int size_warband, unsigned int size_warband_dedicated, unsigned int size_steam = 0)
{
#if defined WARBAND
	unsigned int expected = size_warband;
#elif defined WARBAND_DEDICATED
	unsigned int expected = size_warband_dedicated;
#endif

#if defined WARBAND_STEAM
	if (size_steam)
		expected = size_steam;
#endif

	if (expected == -1 || (expected > 0 && sizeof(T) == expected))
		return;

	char buffer[512];

	sprintf_s(buffer, "Type %s: expected size %d, got %d.\nKABOOM!", typeid(T).name(), expected, sizeof(T));
	MessageBox(nullptr, buffer, "Critical error", MB_ICONERROR);
	exit(1);
}
