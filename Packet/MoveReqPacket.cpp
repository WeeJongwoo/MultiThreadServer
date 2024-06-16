#include "MoveReqPacket.h"

MoveReqPacket::MoveReqPacket(EPacketHeader InHeader, int InX, int InY, int InZ): Packet(InHeader, ENDMARK), X(InX), Y(InY), Z(InZ)
{
	Len = sizeof(Len) + sizeof(Header) + sizeof(EndMark) + sizeof(X) + sizeof(Y), sizeof(Z);
}

MoveReqPacket::~MoveReqPacket()
{
}

void MoveReqPacket::Serialize(char* InSendBuf)
{
	string strX = to_string(this->X);
	string strY = to_string(this->Y);
	string strZ = to_string(this->Z);

	short LenX = (short)strX.size();
	short LenY = (short)strY.size();
	short LenZ = (short)strZ.size();

	int XPos = sizeof(Len) + sizeof(Header);
	int YPos = XPos + sizeof(LenX) + LenX;
	int ZPos = YPos + sizeof(LenY) + LenY;

	Len = (short)(sizeof(Len) + sizeof(Header) + ZPos + sizeof(LenZ) + LenZ + sizeof(EndMark));
	
	memcpy(InSendBuf + XPos , &LenX, sizeof(LenX));
	memcpy(InSendBuf + XPos + sizeof(LenX), strX.c_str(), LenX);
	memcpy(InSendBuf + YPos, &LenY, sizeof(LenY));
	memcpy(InSendBuf + YPos + sizeof(LenY), strY.c_str(), LenY);
	memcpy(InSendBuf + ZPos, &LenZ, sizeof(LenZ));
	memcpy(InSendBuf + ZPos + sizeof(LenZ), strZ.c_str(), LenZ);
	
	memcpy(InSendBuf, &Len, sizeof(Len));
	memcpy(InSendBuf + sizeof(Len), &Header, sizeof(Header));
	memcpy(InSendBuf + Len - sizeof(EndMark), &EndMark, sizeof(EndMark));

}

void MoveReqPacket::Deserialize(char* InRecvBuf)
{
	memcpy(&Len, InRecvBuf, sizeof(Len));

	short LenX;
	short LenY;
	short LenZ;

	//X
	memcpy(&LenX, InRecvBuf + sizeof(Len) + sizeof(Header), sizeof(LenX));
	char* strX = new char[LenX];
	memcpy(strX, InRecvBuf + sizeof(Len) + sizeof(Header) + sizeof(LenX), LenX);
	X = stoi(strX);

	//Y
	memcpy(&LenY, InRecvBuf + sizeof(Len) + sizeof(Header) + sizeof(LenX) + LenX, sizeof(LenY));
	char* strY = new char[LenY];
	memcpy(strY, InRecvBuf + sizeof(Len) + sizeof(Header) + sizeof(LenX) + LenX + sizeof(LenY), LenY);
	Y = stoi(strY);

	//Z
	memcpy(&LenZ, InRecvBuf + sizeof(Len) + sizeof(Header) + sizeof(LenX) + LenX + sizeof(LenY) + LenY, sizeof(LenZ));
	char* strZ = new char[LenZ];
	memcpy(strZ, InRecvBuf + sizeof(Len) + sizeof(Header) + sizeof(LenX) + LenX + sizeof(LenY) + LenY + sizeof(LenZ), LenZ);
	Z = stoi(strZ);

	memcpy(&EndMark, InRecvBuf + Len - sizeof(EndMark), sizeof(EndMark));

	delete[] strX;
	delete[] strY;
	delete[] strZ;
}
