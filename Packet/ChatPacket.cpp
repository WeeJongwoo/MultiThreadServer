#define _CRT_SECURE_NO_WARNINGS
#include "ChatPacket.h"


ChatPacket::ChatPacket(const char* InChat, const char* InID): Packet(EPacketHeader::PK_CHAT_STRING, ENDMARK)
{
	Chat = InChat;
	ChatLen = (short)Chat.size();
	ClientID = InID;
	Len = sizeof(Len) + sizeof(Header) + sizeof(EndMark) + sizeof(ChatLen) + ChatLen;
}

ChatPacket::~ChatPacket()
{
	
}

const char* ChatPacket::GetChat() const
{
	return Chat.c_str();
}

void ChatPacket::Serialize(char* InSendBuf)
{
	short IDLen = (short)ClientID.size();
	int IDPos = sizeof(Len) + sizeof(Header);

	Len = sizeof(Len) + sizeof(EndMark) + sizeof(Header) + sizeof(ChatLen) + ChatLen + sizeof(IDLen) + IDLen;

	memcpy(InSendBuf, &Len, sizeof(Len));
	//memcpy(InSendBuf + sizeof(*InPacketLen), InPacketHeader, sizeof(*InPacketHeader));
	memcpy(InSendBuf + sizeof(Len), &Header, sizeof(Header));

	memcpy(InSendBuf + IDPos, &IDLen, sizeof(IDLen));
	memcpy(InSendBuf + IDPos + sizeof(IDLen), ClientID.c_str(), IDLen);

	int ChatPos = IDPos + sizeof(IDLen) + IDLen;

	memcpy(InSendBuf + ChatPos, &ChatLen, sizeof(ChatLen));
	memcpy(InSendBuf + ChatPos + sizeof(ChatLen), Chat.c_str(), ChatLen);
	memcpy(InSendBuf + Len - 2, &EndMark, sizeof(EndMark));
}

void ChatPacket::Deserialize(char* InRecvBuf)
{
	short IDLen;
	int IDPos = sizeof(Len) + sizeof(Header);
	
	memcpy(&Len, InRecvBuf, sizeof(Len));
	//memcpy(&Header, InRecvdBuf + sizeof(Len), sizeof(Header));

	memcpy(&IDLen, InRecvBuf + IDPos, sizeof(IDLen));
	char* InID = new char[IDLen + 1];
	memcpy(InID, InRecvBuf + IDPos + sizeof(IDLen), IDLen);
	InID[IDLen] = '\0';
	ClientID = InID;

	int ChatPos = IDPos + sizeof(IDLen) + IDLen;
	memcpy(&ChatLen, InRecvBuf + ChatPos, sizeof(ChatLen));
	char* InChat = new char[ChatLen + 1];
	memcpy(InChat, InRecvBuf + ChatPos + sizeof(ChatLen), ChatLen);
	InChat[ChatLen] = '\0';
	Chat = InChat;

	memcpy(&EndMark, InRecvBuf + Len - 2, sizeof(EndMark));

	delete[] InChat;
}
