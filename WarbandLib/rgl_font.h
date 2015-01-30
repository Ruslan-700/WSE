#pragma once

#include "rgl_matrix.h"

namespace rgl
{
	struct glyph
	{
		int u1[7];
	};

	struct font
	{
		int u1;
		int u2;
		float width;
		float height;
		float padding;
		float font_size;
		float size_div_height;
		float size_div_width;
		float font_scale;
		float line_spacing;
		int u3[25];
		glyph glyphs[512];
		int u4;
	};
}
