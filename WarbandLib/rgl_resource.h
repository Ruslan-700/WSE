#pragma once

#include "rgl_string.h"

namespace rgl
{
	struct resource
	{
		string name;
		int in_use;
		int no;
		int base_no;

		virtual ~resource();
		virtual int get_type() const;
	};
}
