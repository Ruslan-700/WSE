#if defined WARBAND
#include "rgl_window_manager.h"

#include "warband.h"

using namespace rgl;

void window_manager::set_tooltip_mesh(mesh *mesh, bool clear_texts)
{
	this->tooltip_timer.update();

	if (this->tooltip_mesh)
		rgl::_delete(this->tooltip_mesh);

	if (!mesh)
	{
		this->tooltip_foreground_color = 0xFFFFFFFF;
		this->tooltip_background_color = 0xFF101010;
	}

	if (clear_texts)
	{
		for (int i = 0; i < 80; ++i)
		{
			this->tooltip_texts[i] = "";
		}

		this->tooltip_max_width = 0.0f;
		this->tooltip_left_offset = -0.03f;
	}

	this->tooltip_mesh = mesh;
	this->tooltip_cur_y = 0.0f;
	this->tooltip_cur_line = 0;
}

void window_manager::set_show_cursor(bool value)
{
	this->show_cursor = value;

	if (warband->graphics_context.dx_device)
		warband->graphics_context.dx_device->ShowCursor(value);
}

void window_manager::display_message(const string &text, const unsigned int &color, const bool &unknown)
{
	THISCALL3(addresses::window_manager_DisplayMessage, this, text, color, unknown)
};
#endif
