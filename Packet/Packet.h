#pragma once
#include "iostream"
#include "PacketHeader.h"

class Packet
{
protected:
	short len;
	PacketHeader header;
	short endMark;

public:

	Packet(PacketHeader InHeader, short InEndMark);
	~Packet();

	short GetLen() { return len; }

	virtual void serialize(char* InBuf);
	virtual void deserialize(const char* InBuf);

};

