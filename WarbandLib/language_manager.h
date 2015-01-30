#pragma once

#include "rgl.h"

namespace wb
{
	struct language_manager
	{
		stl::vector<rgl::string> keys[3571];
		stl::vector<rgl::string> values[3571];
		stl::vector<rgl::string> u1[3571];
		rgl::string not_found_text;
		rgl::string cur_language;
		bool is_russian;

		int get_hash(const rgl::string &str) const;
		const rgl::string &translate(const rgl::string &str);
		const rgl::string *try_translate(const rgl::string &str);
	};
}
