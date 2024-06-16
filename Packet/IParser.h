#pragma once
class IParser
{
public:

	virtual void Serialize(char* InSendBuf) = 0;
	virtual void Deserialize(char* InRecvBuf) = 0;
};

