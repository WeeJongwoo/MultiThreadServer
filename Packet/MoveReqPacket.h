#pragma once
#include "Packet.h"
#include "IParser.h"

class MoveReqPacket :
    public Packet, public IParser
{
    int X;
    int Y;
    int Z;
    string ClientID;

public:
    MoveReqPacket(EPacketHeader InHeader, const char* InID, int InX = 0, int InY = 0, int InZ = 0);
    ~MoveReqPacket();

    int GetX() { return X; }
    int GetY() { return Y; }
    int GetZ() { return Z; }
    const char* GetID() const { return ClientID.c_str(); };

    virtual void Serialize(char* InSendBuf) override;
    virtual void Deserialize(char* InRecvBuf) override;
};

