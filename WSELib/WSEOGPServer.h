#pragma once

#include <Windows.h>
#include "warband.h"

enum WSEOGPServerState
{
	Inactive,
	Starting,
	Listening,
	Stopping,
};

class WSEOGPServer
{
public:
	static DWORD WINAPI StartThread(LPVOID server);

public:
	WSEOGPServer(unsigned short port);
	void Start();
	void Stop();
	void Run();
	void Listen();

private:
	WSEOGPServerState m_state;
	bool m_update;
	unsigned short m_port;
	SOCKET m_socket;
	char m_buffer[64 * 1024];
	sockaddr_in m_from;
	int m_from_len;
	int m_recv_len;
};
