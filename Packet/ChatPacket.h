#pragma once
#include "Packet.h"
#include "IParser.h"

class ChatPacket :
    public Packet, public IParser
{
    short ChatLen;
    string Chat;

public:
    ChatPacket();
    ChatPacket(const char* InChat);
    ~ChatPacket();

    const char* GetChat() const;

    virtual void Serialize(char* InSendBuf) override;
    virtual void Deserialize(char* InRecvBuf) override;
};


