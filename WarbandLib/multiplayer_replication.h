#pragma once

#include "rgl.h"

#define NUM_REPLICATION_TABLES 2

namespace wb
{
	enum replication_table_type
	{
		rtt_agent  = 0,
		rtt_player = 1,
	};

	struct server_replication_table_row
	{
		int v_int;
		int u1;
		float v_float;
		int u2;
		rgl::vector4 v_vec4;
		int u3[4];
		int packet_id;
	};

	struct client_replication_table_row
	{
		int v_int;
		int u1;
		float v_float;
		int u2;
		rgl::vector4 v_vec4;
		int u3[4];
		int packet_id;
		float mission_time;
	};

	struct server_replication
	{
		stl::vector<server_replication_table_row> tables[NUM_REPLICATION_TABLES];
	};

	struct client_replication
	{
		stl::vector<client_replication_table_row> tables[NUM_REPLICATION_TABLES];
	};
}
