#pragma once
#include "Packet.h"
#include "IParser.h"

class ChatPacket :
    public Packet, public IParser
{
    short ChatLen;
    string Chat;
    string ClientID;

public:
    ChatPacket(const char* InChat, const char* InID);
    ~ChatPacket();

    const char* GetChat() const;
    const char* GetID() const { return ClientID.c_str(); };

    virtual void Serialize(char* InSendBuf) override;
    virtual void Deserialize(char* InRecvBuf) override;
};


