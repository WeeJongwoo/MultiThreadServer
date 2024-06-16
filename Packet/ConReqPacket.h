#pragma once
#include "Packet.h"
#include "IParser.h"

class ConReqPacket :
    public Packet, public IParser
{
private:
    string ClientID;

public:
    ConReqPacket(EPacketHeader InHeader, char* InID);
    ~ConReqPacket();

    const char* GetID() const;

    virtual void Serialize(char* InSendBuf) override;
    virtual void Deserialize(char* InRecvBuf) override;
};

