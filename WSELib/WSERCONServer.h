#pragma once

#include <Windows.h>
#include "warband.h"

#define RCON_MAX_CLIENT 10

enum WSERCONServerState
{
	RCONInactive,
	RCONStarting,
	RCONListening,
	RCONStopping,
};


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
	WSERCONServerState m_state;
	unsigned short m_port;
	SOCKET m_socket;
	size_t clientsCnt;
	
};
