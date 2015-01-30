#pragma once

#include "rgl.h"

namespace wb
{
	struct network_client
	{
		double last_receive_time;
		bool kicked;
		rgl::string download_site_id;
		rgl::string download_site_file;
		stl::vector<char> download_data;
		int download_max_chunk;
		float download_chunk_request_last_time;
		stl::vector<float> download_chunk_request_times;
		bool downloading_site;
	};
}
