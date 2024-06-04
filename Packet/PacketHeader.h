#pragma once

#define DATASIZE 30
#define ENDMARK (short)0xffff

enum class PacketHeader : char {
	PK_DATA = 1
};