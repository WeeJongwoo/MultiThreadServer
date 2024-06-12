#include "Packet.h"

Packet::Packet(EPacketHeader InHeader, short InEndMark) : header(InHeader), endMark(InEndMark)
{
}

Packet::~Packet()
{
	len = 0;
}

EPacketHeader* Packet::GetHeader()
{
	return &Header;
}

