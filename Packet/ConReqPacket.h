#pragma once
#include "Packet.h"
#include "IParser.h"

class ConPacket :
    public Packet, public IParser
{
private:
    string ClientID;

public:
    ConPacket(EPacketHeader InHeader, const char* InID);
    ~ConPacket();

    const char* GetID() const { return ClientID.c_str(); };

    virtual void Serialize(char* InSendBuf) override;
    virtual void Deserialize(char* InRecvBuf) override;
};

