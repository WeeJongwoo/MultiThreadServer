#pragma once
#include "iostream"
#include "PacketHeader.h"
#include "string"

using namespace std;

class Packet
{
protected:
	short Len;
	EPacketHeader Header;
	short EndMark;

public:

	Packet(EPacketHeader InHeader, short InEndMark);
	~Packet();

	int GetLen() { return Len; }
	EPacketHeader GetHeader();

};

