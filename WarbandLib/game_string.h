#pragma once

#include "rgl.h"

namespace wb
{
	struct game_string
	{
		rgl::string value;
		rgl::string id;

		const rgl::string &get_text();
	};

	struct string_manager
	{
		game_string *strings;
		game_string *quick_strings;
		int num_strings;
		int num_quick_strings;
	
		const rgl::string &get_operand_string(rgl::string &str, int string_no, int operand_type);

		int addQuickString(const char *str);
		__int64 getQuickStringAsOperand(int index);
	};
}
