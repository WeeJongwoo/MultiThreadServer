#include "MoveReqPacket.h"

MovePacket::MovePacket(EPacketHeader InHeader, const char* InID, int InX, int InY, int InZ): Packet(InHeader, ENDMARK), X(InX), Y(InY), Z(InZ)
{
	ClientID = InID;
	Len = sizeof(Len) + sizeof(Header) + sizeof(EndMark) + sizeof(X) + sizeof(Y), sizeof(Z);
}

MovePacket::~MovePacket()
{
}

void MovePacket::Serialize(char* InSendBuf)
{
	string strX = to_string(this->X);
	string strY = to_string(this->Y);
	string strZ = to_string(this->Z);

	short LenX = (short)strX.size();
	short LenY = (short)strY.size();
	short LenZ = (short)strZ.size();
	short IDLen = (short)ClientID.size();

	int IDPos = sizeof(Len) + sizeof(Header);
	int XPos = IDPos + sizeof(IDLen) + IDLen;
	int YPos = XPos + sizeof(LenX) + LenX;
	int ZPos = YPos + sizeof(LenY) + LenY;

	Len = (short)(sizeof(Len) + sizeof(Header) + ZPos + sizeof(LenZ) + LenZ + sizeof(EndMark));
	
	memcpy(InSendBuf, &Len, sizeof(Len));
	memcpy(InSendBuf + sizeof(Len), &Header, sizeof(Header));
	memcpy(InSendBuf + IDPos, &IDLen, sizeof(IDLen));
	memcpy(InSendBuf + IDPos + sizeof(IDLen), ClientID.c_str(), IDLen);

	memcpy(InSendBuf + XPos, &LenX, sizeof(LenX));
	memcpy(InSendBuf + XPos + sizeof(LenX), strX.c_str(), LenX);
	memcpy(InSendBuf + YPos, &LenY, sizeof(LenY));
	memcpy(InSendBuf + YPos + sizeof(LenY), strY.c_str(), LenY);
	memcpy(InSendBuf + ZPos, &LenZ, sizeof(LenZ));
	memcpy(InSendBuf + ZPos + sizeof(LenZ), strZ.c_str(), LenZ);

	memcpy(InSendBuf + Len - sizeof(EndMark), &EndMark, sizeof(EndMark));

}

void MovePacket::Deserialize(char* InRecvBuf)
{
	memcpy(&Len, InRecvBuf, sizeof(Len));

	short IDLen;
	short LenX;
	short LenY;
	short LenZ;

	int IDPos = sizeof(Len) + sizeof(Header);

	memcpy(&IDLen, InRecvBuf + IDPos, sizeof(IDLen));
	char* InID = new char[IDLen + 1];
	memcpy(InID, InRecvBuf + IDPos + sizeof(IDLen), IDLen);
	InID[IDLen] = '\0';
	ClientID = InID;

	int XPos = IDPos + sizeof(IDLen) + IDLen;

	//X
	memcpy(&LenX, InRecvBuf + XPos, sizeof(LenX));
	char* strX = new char[LenX];
	memcpy(strX, InRecvBuf + XPos + sizeof(LenX), LenX);
	X = stoi(strX);

	int YPos = XPos + sizeof(LenX) + LenX;
	//Y
	memcpy(&LenY, InRecvBuf + YPos, sizeof(LenY));
	char* strY = new char[LenY];
	memcpy(strY, InRecvBuf + YPos + sizeof(LenY), LenY);
	Y = stoi(strY);

	int ZPos = YPos + sizeof(LenY) + LenY;
	//Z
	memcpy(&LenZ, InRecvBuf + ZPos, sizeof(LenZ));
	char* strZ = new char[LenZ];
	memcpy(strZ, InRecvBuf + ZPos + sizeof(LenZ), LenZ);
	Z = stoi(strZ);

	memcpy(&EndMark, InRecvBuf + Len - sizeof(EndMark), sizeof(EndMark));

	delete[] strX;
	delete[] strY;
	delete[] strZ;
}
