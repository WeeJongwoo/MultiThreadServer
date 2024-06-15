#pragma once
#include "Packet.h"
class ConReqPacket :
    public Packet
{
private:
    char ClientID[10];

public:
    ConReqPacket(EPacketHeader InHeader,char* InID);
    ~ConReqPacket();

    const char* GetID() const;

    virtual void Serialize(char* InSendBuf) override;
    virtual void Deserialize(char* InRecvdBuf) override;
};

