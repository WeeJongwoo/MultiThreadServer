#pragma once
#include "Packet.h"
#include "IParser.h"

class ConReqPacket :
    public Packet, public IParser
{
private:
    string ClientID;

public:
    ConReqPacket(EPacketHeader InHeader, const char* InID);
    ~ConReqPacket();

    const char* GetID() const { return ClientID.c_str(); };

    virtual void Serialize(char* InSendBuf) override;
    virtual void Deserialize(char* InRecvBuf) override;
};

