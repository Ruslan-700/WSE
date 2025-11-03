#include "rcon.h"

size_t rconEncode(int id, int type, const char* data, char* buffer, size_t len)
{
	if (len == 0)
	{
		len = strlen(data);
	}
	size_t size = len + 10;

	rconPack(size, buffer);
	rconPack(id, &buffer[4]);
	rconPack(type, &buffer[8]);

	for (size_t i = 0; i < len; ++i)
	{
		buffer[i + 12] = data[i];
	}

	buffer[len + 12] = 0;
	buffer[len + 13] = 0;

	return size + 4;
}

RconPacket rconDecode(char* buffer)
{
	RconPacket packet;

	packet.size = rconUnpack(buffer);
	packet.id = rconUnpack(&buffer[4]);
	packet.type = rconUnpack(&buffer[8]);

	size_t len = packet.size - 10;

	for (size_t i = 0; i < len; ++i)
	{
		packet.data[i] = buffer[i + 12];
	}

	packet.data[len] = '\0';

	return packet;
}

void rconPack(int val, char buffer[4])
{
	for (size_t i = 0; i < 4; ++i)
		buffer[i] = static_cast<unsigned char>((val >> (i * 8)) & 0xFF);
}

int rconUnpack(char buffer[4])
{
	int val = 0;

	for (size_t i = 0; i < 4; ++i)
		val |= (static_cast<unsigned char>(buffer[i]) << (i * 8));

	return val;
}
