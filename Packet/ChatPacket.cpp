#define _CRT_SECURE_NO_WARNINGS
#include "ChatPacket.h"

	
ChatPacket::ChatPacket(): Packet(EPacketHeader::PK_CHAT_STRING, ENDMARK)
{
	Chat = "Chat";
}

ChatPacket::ChatPacket(const char* InChat): Packet(EPacketHeader::PK_CHAT_STRING, ENDMARK)
{
	Chat = InChat;
	ChatLen = Chat.size();

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
	Len = sizeof(Len) + sizeof(EndMark) + sizeof(Header) + sizeof(ChatLen) + ChatLen;

	memcpy(InSendBuf, &Len, sizeof(Len));
	//memcpy(InSendBuf + sizeof(*InPacketLen), InPacketHeader, sizeof(*InPacketHeader));
	memcpy(InSendBuf + sizeof(Len), &Header, sizeof(Header));
	memcpy(InSendBuf + sizeof(Len) + sizeof(Header), &ChatLen, sizeof(ChatLen));
	memcpy(InSendBuf + sizeof(Len) + sizeof(Header) + sizeof(ChatLen), Chat.c_str(), ChatLen);
	memcpy(InSendBuf + Len - 2, &EndMark, sizeof(EndMark));
}

void ChatPacket::Deserialize(char* InRecvBuf)
{

	memcpy(&Len, InRecvBuf, sizeof(Len));
	//memcpy(&Header, InRecvdBuf + sizeof(Len), sizeof(Header));
	memcpy(&ChatLen, InRecvBuf + sizeof(Len) + sizeof(Header), sizeof(ChatLen));
	char* InChat = new char[ChatLen + 1];
	memcpy(InChat, InRecvBuf + sizeof(Len) + sizeof(Header) + sizeof(ChatLen), ChatLen);
	InChat[ChatLen] = '\0';
	Chat = InChat;

	memcpy(&EndMark, InRecvBuf + Len - 2, sizeof(EndMark));

	delete[] InChat;
}
