#include "PacketParser.h"

PacketParser::PacketParser()
{
}

PacketParser::~PacketParser()
{
}

void PacketParser::Serialize(char* InBuf, Packet* InPacket)
{
	//short* InPacketLen = InPacket->GetLen();
	//EPacketHeader* InPacketHeader = InPacket->GetHeader();
	//short* InPacketEndArk = InPacket->GetEndMark();

	//memcpy(InBuf, InPacketLen, sizeof(*InPacketLen));
	//memcpy(InBuf + sizeof(*InPacketLen), InPacketHeader, sizeof(*InPacketHeader));
	//memcpy(InBuf + *InPacketLen - 2, InPacketEndArk, sizeof(*InPacketEndArk));

	//switch (*InPacketHeader)
	//{
	//case EPacketHeader::NOT_INIT:
	//	break;
	//case EPacketHeader::PK_REQ_CON:
	//	break;
	//case EPacketHeader::PK_ACK_CON:
	//	break;
	//case EPacketHeader::PK_REQ_MOVE:
	//	break;
	//case EPacketHeader::PK_ACK_MOVE:
	//	break;
	//case EPacketHeader::PK_CHAT_STRING: // 문자열 길이가 길면 패킷 여러개 보내기, 보낸 시간 순서대로 정렬
	//	break;
	//default:
	//	break;
	//}
}

void PacketParser::Deserialize(char* InBuf, Packet* InPacket)
{
	/*short* InPacketLen = InPacket->GetLen();
	EPacketHeader* InPacketHeader = InPacket->GetHeader();
	short* InPacketEndArk = InPacket->GetEndMark();

	memcpy(InPacketLen, InBuf, sizeof(*InPacketLen));
	memcpy(InPacketHeader, InBuf + sizeof(*InPacketLen), sizeof(*InPacketHeader));
	memcpy(InPacketEndArk, InBuf + *InPacketLen - 2, sizeof(*InPacketEndArk));*/

	/*switch (*InPacketHeader)
	{
	case EPacketHeader::NOT_INIT:
		break;
	case EPacketHeader::PK_REQ_CON:
		break;
	case EPacketHeader::PK_ACK_CON:
		break;
	case EPacketHeader::PK_REQ_MOVE:
		break;
	case EPacketHeader::PK_ACK_MOVE:
		break;
	case EPacketHeader::PK_CHAT_STRING:
		break;
	default:
		break;
	}*/
}

EPacketHeader PacketParser::GetHeader(char* InBuf)
{
	EPacketHeader InBufHeader;

	memcpy(&InBufHeader, InBuf + 2, sizeof(InBufHeader));

	return InBufHeader;
}
