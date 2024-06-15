#define _CRT_SECURE_NO_WARNINGS
#include "ChatPacket.h"

	
ChatPacket::ChatPacket(const char* InChat, int InChatLen): Packet(EPacketHeader::PK_CHAT_STRING, ENDMARK), Chat(new char[InChatLen])
{
	
	strcpy(Chat, InChat);
	ChatLen = InChatLen;

	Len = sizeof(Len) + sizeof(Header) + sizeof(EndMark) + sizeof(ChatLen) + sizeof(*Chat);
}

ChatPacket::~ChatPacket()
{
	delete Chat;
}

void ChatPacket::Serialize(char* InSendBuf)
{
}

void ChatPacket::Deserialize(char* InRecvBuf)
{
}
