#include "Packet.h"

Packet::Packet(PacketHeader InHeader, short InEndMark) : header(InHeader), endMark(InEndMark)
{
}

Packet::~Packet()
{
	len = 0;
}

void Packet::serialize(char* InBuf)
{
	memcpy(InBuf, &(this->len), sizeof(len));
	memcpy(InBuf + sizeof(len), &(this->header), sizeof(header));
	memcpy(InBuf + this->len - 2, &(this->endMark), sizeof(endMark));
}

void Packet::deserialize(const char* InBuf)
{
	memcpy(&this->len, InBuf, sizeof(len));
	memcpy(&this->header, InBuf + sizeof(len), sizeof(header));
	memcpy(&this->endMark, InBuf + len - 2, sizeof(endMark));
}
