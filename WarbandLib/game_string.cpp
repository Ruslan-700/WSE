#include "game_string.h"

#include "warband.h"

using namespace wb;

const rgl::string &game_string::get_text()
{
	const rgl::string *translated = warband->language_manager.try_translate(this->id);

	return translated ? *translated : this->value;
}

const rgl::string &string_manager::get_operand_string(rgl::string &str, int string_no, int operand_type)
{
	if (operand_type == 0)
	{
		if (string_no < 0 || string_no > NUM_REGISTERS)
			str = "Invalid string register!";
		else
			str = warband->basic_game.string_registers[string_no];
	}
	else
	{
		if (operand_type == 22)
		{
			if (string_no < 0 || string_no > this->num_quick_strings)
				str = "Invalid quick string!";
			else
				str = this->quick_strings[string_no].get_text();
		}
		else
		{
			if (string_no < 0 || string_no > this->num_strings)
				str = "Invalid string!";
			else
				str = this->strings[string_no].get_text();
		}
	}

	return str;
};
