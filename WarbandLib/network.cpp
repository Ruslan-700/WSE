#include "network.h"

#include "warband.h"

using namespace wb;
#if defined WARBAND
void network_manager::connect_to_server(const unsigned __int64 &vac_id, const rgl::string &ip, const int &port, const rgl::string &password)
{
	THISCALL4(wb::addresses::network_manager_ConnectToServer, this, vac_id, ip, port, password)
}
#endif