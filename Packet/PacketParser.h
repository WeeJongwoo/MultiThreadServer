#pragma once
#include "Packet.h"
#include "PacketHeader.h"

class PacketParser
{

public:
	PacketParser();
	~PacketParser();

	void Serialize(char* InBuf, Packet* InPacket);
	void Deserialize(char* InBuf, Packet* InPacket);
	EPacketHeader GetHeader(char* InBuf);
};

