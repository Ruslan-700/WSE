#ifndef FUNCS_H
#define FUNCS_H

#include <winsock.h>

#define SERVERDATA_AUTH 3
#define SERVERDATA_AUTH_RESPONSE 2
#define SERVERDATA_EXECCOMMAND 2
#define SERVERDATA_RESPONSE_VALUE 0

struct RconPacket
{
	int size;
	int id;
	int type;
	char data[4096];
};

size_t rconEncode(int id, int type, const char* data, char* buffer, size_t len = 0);
RconPacket rconDecode(char* buffer);
void rconPack(int val, char buffer[4]);
int rconUnpack(char buffer[4]);

#endif
