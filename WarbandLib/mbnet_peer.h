#pragma once

#include "rgl.h"
#include "mbnet.h"
#include "network_buffer.h"

namespace wb
{
	struct mbnet_peer
	{
		short u1;
		int u2;
		mbnet_address address;
		network_buffer outgoing_packets[20];
		int num_outgoing_packets;
		network_buffer incoming_packets[20];
		int num_incoming_packets;
	};
}
