#pragma once
#include "iostream"
#include "PacketHeader.h"

class Packet
{
protected:
	short len;
	EPacketHeader Header;
	short endMark;

public:

	Packet(EPacketHeader InHeader, short InEndMark);
	~Packet();

	short* GetLen() { return &len; }
	EPacketHeader* GetHeader();
	short* GetEndMark() { return &endMark; };

};

