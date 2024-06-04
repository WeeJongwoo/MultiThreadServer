#pragma once
#include "Packet.h"


class StringPacket : public Packet
{
private:
	
	char Data[20];

public:
	StringPacket(const char* InData);
	~StringPacket();
	const char* GetData() const { return this->Data; };

	virtual void serialize(char* InBuf) override;
	virtual void deserialize(const char* InBuf) override;

};

