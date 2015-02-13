#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include "dinput.h"
#include "rgl.h"

namespace wb
{
	struct input_manager
	{
#if defined WARBAND	
		IDirectInput8 *di_iface;
		IDirectInputDevice8 *di_keyboard;
		IDirectInputDevice8 *di_mouse;
#endif		
		int cur_key_status[256];
		int prev_key_status[256];
		int stored_key_status[256];
		float mouse_x_movement;
		float mouse_y_movement;
		int mouse_x;
		int mouse_y;
		rgl::timer activity_timer;
		rgl::timer scroll_timer;
		bool active;
		int mouse_num_decays;
		stl::vector<unsigned int> omitted_keys;
		int mouse_cur_x;
		int mouse_cur_y;
		int mouse_cur_z;
#if defined WARBAND_DEDICATED
		int u1;
#else
		//int u1[12];
		int u1[14];
#endif

		bool is_key_down(unsigned int key);
		bool is_key_clicked(unsigned int key);
		bool is_key_released(unsigned int key);
	};
}
