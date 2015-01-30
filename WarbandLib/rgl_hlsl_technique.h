#pragma once

#include "d3dx9.h"
#include "rgl_resource.h"

namespace rgl
{
	enum hlsl_technique_flags
	{
		htf_hdr = 0x1,
	};

	enum hlsl_technique_kind
	{
		htk_none          = 0,
		htk_shadow        = 1,
		htk_shadow_nvidia = 2,
		htk_preshaded     = 3,
		htk_hdr           = 4,
	};

	struct hlsl_technique : resource
	{
		bool initialized;
		unsigned int flags;
		D3DXHANDLE dx_handles[5];
		int u1;
	};
}
