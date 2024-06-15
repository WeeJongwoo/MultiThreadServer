#define _CRT_SECURE_NO_WARNINGS
#include "ConReqPacket.h"


ConReqPacket::ConReqPacket(EPacketHeader InHeader, char* InID): Packet(InHeader,ENDMARK)
{
	strcpy(ClientID, InID);
	Len = sizeof(Len) + sizeof(EndMark) + sizeof(Header) + sizeof(ClientID);
}

ConReqPacket::~ConReqPacket()
{
}

const char* ConReqPacket::GetID() const
{
	return ClientID;
}

void ConReqPacket::Serialize(char* InSendBuf)
{
	//short* InPacketLen = &this->Len;
	//EPacketHeader* InPacketHeader = &this->Header;
	//short* InPacketEndArk = &this->EndMark;

	memcpy(InSendBuf, &Len, sizeof(Len));
	//memcpy(InSendBuf + sizeof(*InPacketLen), InPacketHeader, sizeof(*InPacketHeader));
	memcpy(InSendBuf + Len - 2, &EndMark, sizeof(EndMark));
	memcpy(InSendBuf + sizeof(Len) + sizeof(Header), this->ClientID, sizeof(ClientID));
}

void ConReqPacket::Deserialize(char* InRecvdBuf)
{
	//short* InPacketLen = &this->Len;
	//EPacketHeader* InPacketHeader = &this->Header;
	//short* InPacketEndArk = &this->EndMark;

	memcpy(&Len, InRecvdBuf, sizeof(Len));
	//memcpy(&Header, InRecvdBuf + sizeof(Len), sizeof(Header));
	memcpy(&EndMark, InRecvdBuf + Len - 2, sizeof(EndMark));
	memcpy(ClientID, InRecvdBuf + sizeof(Len) + sizeof(Header), sizeof(ClientID));
}
