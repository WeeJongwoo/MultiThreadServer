#pragma once
#include "Packet.h"

class ChatPacket :
    public Packet
{
    short ChatLen;
    char* Chat;

public:
    ChatPacket(const char* InChat, int InChatLen);
    ~ChatPacket();

    virtual void Serialize(char* InSendBuf);
    virtual void Deserialize(char* InRecvBuf);
};


