#include "WSERCONServer.h"

#if defined WARBAND_DEDICATED

#include <cstdio>
#include <cstring>
#include <ctime>
#include "WSE.h"
#include "rcon/ws-util.h"
#include "rcon/rcon.h"


DWORD WINAPI WSERCONServer::StartThread(LPVOID server)
{
	((WSERCONServer *)server)->Listen();
	return 0;
}

WSERCONServer::WSERCONServer(unsigned short port, std::string password)
{
	m_port = port;
	m_password = password;
	m_state = RCONInactive;
	m_remote_command = false;
	m_thread = NULL;
	m_socket = INVALID_SOCKET;
	m_clientsCnt = 0;
}

void WSERCONServer::Start()
{
	if (m_state == RCONStarting || m_state == RCONListening)
		return;

	while (m_state == RCONStopping)
	{
		Sleep(20);
	}

	m_state = RCONStarting;

	sockaddr_in addr;
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		WSE->Log.Error("RCON server: error %d while starting up Winsock", WSAGetLastError());
		m_state = RCONInactive;
		return;
	}

	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (m_socket == INVALID_SOCKET)
	{
		WSE->Log.Error("RCON server: error %d while creating socket", WSAGetLastError());
		m_state = RCONInactive;
		return;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_port);

	if (bind(m_socket, (sockaddr *)&addr, sizeof(addr)))
	{
		WSE->Log.Error("RCON server: error %d while binding socket", WSAGetLastError());
		m_state = RCONInactive;
		return;
	}

	WSE->Log.Info("RCON server: listening on port %hd", m_port);

	//Listen to incoming connections
	listen(m_socket, SOMAXCONN);

	m_thread = CreateThread(nullptr, 0, StartThread, this, 0, nullptr);
}

void WSERCONServer::Stop()
{
	m_state = RCONStopping;
	shutdown(m_socket, SD_BOTH);
	closesocket(m_socket);
	m_socket = INVALID_SOCKET;

	if (m_thread != NULL)
	{
		WaitForSingleObject(m_thread, 3000);
		CloseHandle(m_thread);
		m_thread = NULL;
	}

	CloseAllConnections();
	WSACleanup();
	WSE->Log.Info("RCON server: stopped listening");
	m_state = RCONInactive;
}

void WSERCONServer::CloseAllConnections()
{
	for (ConnectionList::iterator it = m_connections.begin(); it != m_connections.end(); ++it)
	{
		ShutdownConnection(it->sd);
	}
	m_connections.clear();
	m_clientsCnt = 0;
}

void WSERCONServer::ResetAuthorizationConnections()
{
	ConnectionList::iterator it = m_connections.begin();
	while (it != m_connections.end()) {
		it->auth = false;
		++it;
	}
}

void WSERCONServer::SetupFDSets(fd_set& ReadFDs, fd_set& WriteFDs,
	fd_set& ExceptFDs)
{
	FD_ZERO(&ReadFDs);
	FD_ZERO(&WriteFDs);
	FD_ZERO(&ExceptFDs);

	// Add the listener socket to the read and except FD sets
	if (m_socket != INVALID_SOCKET) {
		FD_SET(m_socket, &ReadFDs);
		FD_SET(m_socket, &ExceptFDs);
	}

	// Add client connections
	ConnectionList::iterator it = m_connections.begin();
	while (it != m_connections.end()) {
		FD_SET(it->sd, &ReadFDs);
		FD_SET(it->sd, &ExceptFDs);
		++it;
	}
}

bool WSERCONServer::RecvAll(SOCKET sd, char* buffer, int size)
{
	int received = 0;
	int attempts = 0;

	while (received < size)
	{
		int n = recv(sd, buffer + received, size - received, 0);

		if (n > 0)
		{
			received += n;
			attempts = 0;
		}
		else if (n == 0)
		{
			return false;
		}
		else
		{
			int err = WSAGetLastError();
			if (err == WSAEWOULDBLOCK)
			{
				if (++attempts > 1000)
					return false;
				Sleep(1);
				continue;
			}
			return false;
		}
	}

	return true;
}

bool WSERCONServer::ReadRcon(Connection& conn)
{
	char buffer[4100];
	int nBytes = recv(conn.sd, buffer, 4, 0);
	if (nBytes == 0) {
		return false;
	}
	else if (nBytes == SOCKET_ERROR) {
		int err = WSAGetLastError();
		return (err == WSAEWOULDBLOCK);
	}
	else if (nBytes < 4) {
		if (!RecvAll(conn.sd, buffer + nBytes, 4 - nBytes))
			return false;
	}

	int size = rconUnpack(buffer);
	if (size < 10 || size > 4096)
	{
		return false;
	}

	if (!RecvAll(conn.sd, &buffer[4], size))
	{
		return false;
	}

	RconPacket packet = rconDecode(buffer);

	if (packet.type == SERVERDATA_AUTH)
	{
		size = rconEncode(packet.id, SERVERDATA_RESPONSE_VALUE, "", buffer);
		send(conn.sd, buffer, size, 0);

		if (strcmp(packet.data, m_password.c_str()) == 0)
		{
			size = rconEncode(packet.id, SERVERDATA_AUTH_RESPONSE, "", buffer);
			conn.auth = true;
			WSE->Log.Info("RCON server: Auth request: Accepted");
		}
		else
		{
			size = rconEncode(-1, SERVERDATA_AUTH_RESPONSE, "", buffer);
			conn.auth = false;
			WSE->Log.Info("RCON server: Auth request: Declined");
		}

		send(conn.sd, buffer, size, 0);
	}
	else if (packet.type == SERVERDATA_EXECCOMMAND)
	{
		if (conn.auth == true)
		{
			rgl::string command = packet.data;
			WSE->Log.Info("RCON server: Command : %s", command.length() > 1024 ? command.substr(0, 1024).c_str() : command.c_str());

			rgl::string message = "";
			EnterCriticalSection(&warband->network_manager.network_critical_section);
			m_remote_command = true;
			WSE->Game.ExecuteConsoleCommand(message, command);
			LeaveCriticalSection(&warband->network_manager.network_critical_section);

			if (message.str_length > 0)
			{
				rgl::string chunk = "";
				int offset = 0;

				while (offset < message.length())
				{
					chunk = message.substr(offset, rglMin(offset + 4086, message.length()));
					size = rconEncode(packet.id, SERVERDATA_RESPONSE_VALUE, chunk.c_str(), buffer);
					send(conn.sd, buffer, size, 0);

					offset += 4086;
				}
			}
			else
			{
				size = rconEncode(packet.id, SERVERDATA_RESPONSE_VALUE, "", buffer);
				send(conn.sd, buffer, size, 0);
			}
		}
		else
		{
			size = rconEncode(packet.id, SERVERDATA_RESPONSE_VALUE, "", buffer);
			send(conn.sd, buffer, size, 0);
		}
	}
	else if (packet.type == SERVERDATA_RESPONSE_VALUE)
	{
		char response[4] = { 0x00, 0x01, 0x00, 0x00 };
		size = rconEncode(packet.id, SERVERDATA_RESPONSE_VALUE, response, buffer, 4);
		send(conn.sd, buffer, size, 0);
	}

	return true;
}

void WSERCONServer::Listen()
{
	m_state = RCONListening;
	sockaddr_in sinRemote;
	int nAddrSize = sizeof(sinRemote);
	m_clientsCnt = 0;

	while (m_state == RCONListening)
	{
		fd_set ReadFDs, WriteFDs, ExceptFDs;
		SetupFDSets(ReadFDs, WriteFDs, ExceptFDs);

		timeval timeout;
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		int selectResult = select(0, &ReadFDs, &WriteFDs, &ExceptFDs, &timeout);

		if (selectResult == 0)
			continue;

		if (selectResult < 0) {
			if (m_state != RCONListening)
				break;
			WSE->Log.Error("RCON server: %s", WSAGetLastErrorMessage("select() failed"));
			break;
		}

		//// Something happened on one of the sockets.
		// Was it the listener socket?...
		if (FD_ISSET(m_socket, &ReadFDs)) {
			SOCKET sd = accept(m_socket,
				(sockaddr*)&sinRemote, &nAddrSize);
			if (sd != INVALID_SOCKET) {

				if (m_clientsCnt < RCON_MAX_CLIENT)
				{
					WSE->Log.Info("RCON server: Accepted connection from %s:%d", inet_ntoa(sinRemote.sin_addr), ntohs(sinRemote.sin_port));

					m_connections.push_back(Connection(sd));
					++m_clientsCnt;

					// Mark the socket as non-blocking, for safety.
					u_long nNoBlock = 1;
					ioctlsocket(sd, FIONBIO, &nNoBlock);
				}
				else
				{
					ShutdownConnection(sd);
					WSE->Log.Info("RCON server: Connection from %s:%d dropped from server because reached limit of clients", inet_ntoa(sinRemote.sin_addr), ntohs(sinRemote.sin_port));
				}

			}
			else {
				if (m_state != RCONListening)
					break;
				WSE->Log.Error("RCON server: %s", WSAGetLastErrorMessage("accept() failed"));
				break;
			}
		}
		else if (FD_ISSET(m_socket, &ExceptFDs)) {
			int err;
			int errlen = sizeof(err);
			getsockopt(m_socket, SOL_SOCKET, SO_ERROR,
				(char*)&err, &errlen);
			WSE->Log.Error("RCON server: %s", WSAGetLastErrorMessage("Exception on listening socket : ", err));
			break;
		}

		// ...Or was it one of the client sockets?
		ConnectionList::iterator it = m_connections.begin();
		while (it != m_connections.end()) {
			bool bOK = true;
			const char* pcErrorType = 0;

			// See if this socket's flag is set in any of the FD sets.
			if (FD_ISSET(it->sd, &ExceptFDs)) {
				bOK = false;
				pcErrorType = "General socket error";
				FD_CLR(it->sd, &ExceptFDs);
			}
			else {
				if (FD_ISSET(it->sd, &ReadFDs)) {
					bOK = ReadRcon(*it);
					pcErrorType = "Read error";
					FD_CLR(it->sd, &ReadFDs);
				}
			}

			if (!bOK) {
				// Something bad happened on the socket, or the
				// client closed its half of the connection.  Shut
				// the conn down and remove it from the list.
				int err;
				int errlen = sizeof(err);
				getsockopt(it->sd, SOL_SOCKET, SO_ERROR,
					(char*)&err, &errlen);
				if (err != NO_ERROR) {
					WSE->Log.Error("RCON server: %s", WSAGetLastErrorMessage(pcErrorType, err));
				}
				ShutdownConnection(it->sd);
				it = m_connections.erase(it);
				--m_clientsCnt;
			}
			else {
				// Go on to next connection
				++it;
			}
		}
	}

	m_state = RCONInactive;
}

#endif
