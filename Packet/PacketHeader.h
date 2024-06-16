#pragma once

#define DATASIZE 30
#define ENDMARK (short)0xffff

enum class EPacketHeader : char {
	NOT_INIT = 0,
	PK_REQ_CON,
	PK_ACK_CON,
	PK_REQ_MOVE,
	PK_ACK_MOVE,
	PK_CHAT_STRING,
	PK_EXIT
};

//EPacketHeader GetBufHeader(char* InBuf)
//{
//	EPacketHeader InHeader;
//
//	memcpy(&InHeader, InBuf + 2, sizeof(InHeader));
//	return InHeader;
//}