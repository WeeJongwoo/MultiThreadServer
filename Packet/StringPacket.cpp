#define _CRT_SECURE_NO_WARNINGS
#include "StringPacket.h"

StringPacket::StringPacket(const char* InData) : Packet(PacketHeader::PK_DATA, ENDMARK)
{
	strcpy(Data, InData);

	this->len = sizeof(len) + sizeof(header) + sizeof(Data) + sizeof(endMark);
}

StringPacket::~StringPacket()
{
	
}

void StringPacket::serialize(char* InBuf)
{
	this->Packet::serialize(InBuf);
	memcpy(InBuf + sizeof(len) + sizeof(header), Data, sizeof(Data));
}

void StringPacket::deserialize(const char* InBuf)
{
	Packet::deserialize(InBuf);
	memcpy(&this->Data, InBuf + sizeof(len) + sizeof(header), sizeof(Data));
}

