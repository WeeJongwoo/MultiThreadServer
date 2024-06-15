#pragma once
#include "Packet.h"

class MoveReqPacket :
    public Packet
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

    virtual void Serialize(char* InSendBuf);
    virtual void Deserialize(char* InRecvBuf);
};

