#pragma once
#include "iostream"
#include "Packet"

class Packet
{
protected:
	unsigned short len;
	PacketHeader header;
	unsigned short endMark;

public:

	Packet(PacketHeader InHeader, short InEndMark);
	~Packet();



};

