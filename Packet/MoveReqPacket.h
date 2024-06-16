#pragma once
#include "Packet.h"
#include "IParser.h"

class MoveReqPacket :
    public Packet, public IParser
{
    int X;
    int Y;
    int Z;

public:
    MoveReqPacket(EPacketHeader InHeader, int InX = 0, int InY = 0, int InZ = 0);
    ~MoveReqPacket();

    int GetX() { return X; }
    int GetY() { return Y; }
    int GetZ() { return Z; }

    virtual void Serialize(char* InSendBuf) override;
    virtual void Deserialize(char* InRecvBuf) override;
};

