#pragma once

#include "rgl.h"
#include "mbnet.h"

namespace wb
{
	struct mbnet_host
	{
		SOCKET udp_socket;
		SOCKET tcp_socket;
		mbnet_address address;
		mbnet_address recv_address;
		int recv_buffer[1024];
		int recv_num_bytes;
	};
}
