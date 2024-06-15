#include "Packet.h"

Packet::Packet(EPacketHeader InHeader, short InEndMark) : Header(InHeader), EndMark(InEndMark)
{
	Len = sizeof(this);
}

Packet::~Packet()
{
	Len = 0;
}

EPacketHeader Packet::GetHeader()
{
	return Header;
}
