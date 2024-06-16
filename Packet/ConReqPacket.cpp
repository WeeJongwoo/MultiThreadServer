#define _CRT_SECURE_NO_WARNINGS
#include "ConReqPacket.h"


ConPacket::ConPacket(EPacketHeader InHeader, const char* InID): Packet(InHeader,ENDMARK), ClientID(new char[strlen(InID) + 1])
{
	ClientID = InID;

	Len = sizeof(Len) + sizeof(EndMark) + sizeof(Header) + ClientID.size();
}

ConPacket::~ConPacket()
{
}

void ConPacket::Serialize(char* InSendBuf)
{
	//short* InPacketLen = &this->Len;
	//EPacketHeader* InPacketHeader = &this->Header;
	//short* InPacketEndArk = &this->EndMark;
	short IDLen = ClientID.size();
	Len = sizeof(Len) + sizeof(EndMark) + sizeof(Header) + sizeof(IDLen) + IDLen;

	memcpy(InSendBuf, &Len, sizeof(Len));
	//memcpy(InSendBuf + sizeof(*InPacketLen), InPacketHeader, sizeof(*InPacketHeader));
	memcpy(InSendBuf + sizeof(Len), &Header, sizeof(Header));
	memcpy(InSendBuf + sizeof(Len) + sizeof(Header), &IDLen, sizeof(IDLen));
	memcpy(InSendBuf + sizeof(Len) + sizeof(Header) + sizeof(IDLen), ClientID.c_str(), IDLen);
	memcpy(InSendBuf + Len - 2, &EndMark, sizeof(EndMark));
}

void ConPacket::Deserialize(char* InRecvBuf)
{
	//short* InPacketLen = &this->Len;
	//EPacketHeader* InPacketHeader = &this->Header;
	//short* InPacketEndArk = &this->EndMark;
	short IDLen;

	memcpy(&Len, InRecvBuf, sizeof(Len));
	//memcpy(&Header, InRecvdBuf + sizeof(Len), sizeof(Header));
	memcpy(&IDLen, InRecvBuf + sizeof(Len) + sizeof(Header), sizeof(IDLen));
	char* InID = new char[IDLen+1];
	memcpy(InID, InRecvBuf + sizeof(Len) + sizeof(Header) + sizeof(IDLen), IDLen);
	InID[IDLen] = '\0';
	ClientID = InID;

	memcpy(&EndMark, InRecvBuf + Len - 2, sizeof(EndMark));

	delete[] InID;
}
