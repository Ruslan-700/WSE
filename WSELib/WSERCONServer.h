#pragma once

#if defined WARBAND_DEDICATED

#include <Windows.h>
#include <vector>
#include "warband.h"

#define RCON_MAX_CLIENT 10

enum WSERCONServerState
{
	RCONInactive,
	RCONStarting,
	RCONListening,
	RCONStopping,
};

struct Connection {
	SOCKET sd;
	bool auth;

	Connection(SOCKET sd_) : sd(sd_), auth(false) { }
};
typedef std::vector<Connection> ConnectionList;

class WSERCONServer
{
public:
	static DWORD WINAPI StartThread(LPVOID server);
	std::string m_password;
	bool m_remote_command;

public:
	WSERCONServer(unsigned short port, std::string password);
	void Start();
	void Stop();
	void Listen();
	void ResetAuthorizationConnections();

private:
	void SetupFDSets(fd_set& ReadFDs, fd_set& WriteFDs, fd_set& ExceptFDs);
	bool ReadRcon(Connection& conn);
	static bool RecvAll(SOCKET sd, char* buffer, int size);
	void CloseAllConnections();

	volatile WSERCONServerState m_state;
	unsigned short m_port;
	SOCKET m_socket;
	HANDLE m_thread;
	ConnectionList m_connections;
	size_t m_clientsCnt;
};

#endif
