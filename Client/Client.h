#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include "iostream"
#include "limits"
#include "ctime"
#include "../Packet/ConReqPacket.h"
#include "../Packet/MoveReqPacket.h"
#include "../Packet/ChatPacket.h"

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    512


class Client
{

protected:
	LPVOID lpMsgBuf;
	WSADATA wsa;
	SOCKET sock;
	SOCKADDR_IN serveraddr;

	char buf[BUFSIZE + 1];
	int len;
	int retval;

	string ID;

	static HANDLE Mutex;

public:
	Client();
	~Client();

	void err_quit(char* msg);
	void err_display(char* msg);
	void Init();
	void Communication();

	static DWORD WINAPI ProcSend(LPVOID lpParam);
	static DWORD WINAPI ProcRecv(LPVOID lpParam);

};

