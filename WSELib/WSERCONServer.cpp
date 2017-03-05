#include "WSERCONServer.h"

#include <cstdio>
#include <cstring>
#include <ctime>
#include "WSE.h"
#include "rcon/ws-util.h"
#include "rcon/rcon.h"

struct Connection {
	SOCKET sd;
	bool auth;

	Connection(SOCKET sd_) : sd(sd_) { }
};
typedef std::vector<Connection> ConnectionList;

ConnectionList gConnections;


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

	CreateThread(nullptr, 0, StartThread, this, 0, nullptr);
}

void WSERCONServer::Stop()
{
	m_state = RCONStopping;
	shutdown(m_socket, SD_BOTH);
	closesocket(m_socket);
	WSACleanup();
	WSE->Log.Info("RCON server: stopped listening");
	m_state = RCONInactive;
}

void WSERCONServer::ResetAuthorizationConnections()
{
	ConnectionList::iterator it = gConnections.begin();
	while (it != gConnections.end()) {
		it->auth = false;
		++it;
	}
}

void SetupFDSets(fd_set& ReadFDs, fd_set& WriteFDs,
	fd_set& ExceptFDs, SOCKET ListeningSocket = INVALID_SOCKET)
{
	FD_ZERO(&ReadFDs);
	FD_ZERO(&WriteFDs);
	FD_ZERO(&ExceptFDs);

	// Add the listener socket to the read and except FD sets, if there
	// is one.
	if (ListeningSocket != INVALID_SOCKET) {
		FD_SET(ListeningSocket, &ReadFDs);
		FD_SET(ListeningSocket, &ExceptFDs);
	}

	// Add client connections
	ConnectionList::iterator it = gConnections.begin();
	while (it != gConnections.end()) {
		FD_SET(it->sd, &ReadFDs);
		FD_SET(it->sd, &ExceptFDs);
		++it;
	}
}

bool ReadRcon(Connection& conn)
{
	char buffer[4100];
	char ch;
	int nBytes = recv(conn.sd, buffer, 4, 0);
	if (nBytes == 0) {
		return false;
	}
	else if (nBytes == SOCKET_ERROR) {
		int err;
		int errlen = sizeof(err);
		getsockopt(conn.sd, SOL_SOCKET, SO_ERROR, (char*)&err, &errlen);
		return (err == WSAEWOULDBLOCK);
	}

	long size = rconUnpack(buffer) + 4;
	if (size < 10 || size > 4096)
	{
		return true;
	}

	while (nBytes < size)
	{
		int nBytes1 = recv(conn.sd, &ch, sizeof(char), 0);
		if (nBytes1 <= 0) 
		{
			return true;
		}
		buffer[nBytes] = ch;
		++nBytes;
	}

	RconPacket packet = rconDecode(buffer);

	if (packet.type == SERVERDATA_AUTH)
	{
		size_t size = rconEncode(packet.id, SERVERDATA_RESPONSE_VALUE, "", buffer);
		int nBytes = send(conn.sd, buffer, size, 0);

		if (strcmp(packet.data, WSE->Network.m_rcon_server->m_password.c_str()) == 0)
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

		nBytes = send(conn.sd, buffer, size, 0);

	}
	else if (packet.type == SERVERDATA_EXECCOMMAND)
	{
		if (conn.auth == true)
		{
			WSE->Log.Info("RCON server: Command : %s", packet.data);

			rgl::string command = packet.data;
			rgl::string message = "";
			
			EnterCriticalSection(&warband->network_manager.network_critical_section);
			WSE->Network.m_rcon_server->m_remote_command = true;
			WSE->Game.ExecuteConsoleCommand(message, command);
			LeaveCriticalSection(&warband->network_manager.network_critical_section);

			if (message.str_length > 0)
			{
				rgl::string chunk = "";
				int offset = 0;
				int end = 0;
				int remain = message.str_length;
				while (remain > 0)
				{	
					end = offset + 4086;
					if (end > message.str_length)
					{
						end = message.str_length;
					}

					chunk = message.substr(offset, end);
					size = rconEncode(packet.id, SERVERDATA_RESPONSE_VALUE, chunk.c_str(), buffer);
					nBytes = send(conn.sd, buffer, size, 0);
					
					offset += 4086;
					remain -= 4086;
				}
				
			}
			else
			{
				size = rconEncode(packet.id, SERVERDATA_RESPONSE_VALUE, "", buffer);
				nBytes = send(conn.sd, buffer, size, 0);
			}

		}
		else
		{
			size = rconEncode(packet.id, SERVERDATA_RESPONSE_VALUE, "", buffer);
			nBytes = send(conn.sd, buffer, size, 0);
		}
		
	}
	else if (packet.type == SERVERDATA_RESPONSE_VALUE)
	{
		char response[4] = { 0x00, 0x01, 0x00, 0x00 };
		size = rconEncode(packet.id, SERVERDATA_RESPONSE_VALUE, response, buffer, 4);
		nBytes = send(conn.sd, buffer, size, 0);
	}

	return true;
}

void WSERCONServer::Listen()
{
	m_state = RCONListening;
	sockaddr_in sinRemote;
	int nAddrSize = sizeof(sinRemote);
	size_t clientsCnt = 0;

	while (m_state == RCONListening)
	{
		fd_set ReadFDs, WriteFDs, ExceptFDs;
		SetupFDSets(ReadFDs, WriteFDs, ExceptFDs, m_socket);

		if (select(0, &ReadFDs, &WriteFDs, &ExceptFDs, 0) > 0) {
			//// Something happened on one of the sockets.
			// Was it the listener socket?...
			if (FD_ISSET(m_socket, &ReadFDs)) {
				SOCKET sd = accept(m_socket,
					(sockaddr*)&sinRemote, &nAddrSize);
				if (sd != INVALID_SOCKET) {
					
					if (clientsCnt < RCON_MAX_CLIENT)
					{
						WSE->Log.Info("RCON server: Accepted connection from %s:%d", inet_ntoa(sinRemote.sin_addr), ntohs(sinRemote.sin_port));

						gConnections.push_back(Connection(sd));
						++clientsCnt;

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
					WSE->Log.Error("RCON server: %s", WSAGetLastErrorMessage("accept() failed"));
					return;
				}
			}
			else if (FD_ISSET(m_socket, &ExceptFDs)) {
				int err;
				int errlen = sizeof(err);
				getsockopt(m_socket, SOL_SOCKET, SO_ERROR,
					(char*)&err, &errlen);
				WSE->Log.Error("RCON server: %s", WSAGetLastErrorMessage("Exception on listening socket : ", err));
				return;
			}

			// ...Or was it one of the client sockets?
			ConnectionList::iterator it = gConnections.begin();
			while (it != gConnections.end()) {
				bool bOK = true;
				const char* pcErrorType = 0;

				// See if this socket's flag is set in any of the FD
				// sets.
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
					gConnections.erase(it);
					--clientsCnt;
					it = gConnections.begin();
				}
				else {
					// Go on to next connection
					++it;
				}
			}
		}
		else {
			WSE->Log.Error("RCON server: %s", WSAGetLastErrorMessage("select() failed"));
			return;
		}
	}
	
	m_state = RCONInactive;
}
