#include "input_manager.h"

using namespace wb;

bool input_manager::is_key_down(unsigned int key)
{
	return key >= 0 && key < 256 && this->cur_key_status[key];
}

bool input_manager::is_key_clicked(unsigned int key)
{
	return key >= 0 && key < 256 && this->cur_key_status[key] && !this->prev_key_status[key];
}

bool input_manager::is_key_released(unsigned int key)
{
	return key >= 0 && key < 256 && !this->cur_key_status[key] && this->prev_key_status[key];
}
