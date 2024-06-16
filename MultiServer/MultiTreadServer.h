#pragma once

#pragma comment(lib,"ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include<WinSock2.h>
#include<stdlib.h>
#include<iostream>
#include<tchar.h>
#include "vector"
#include "../Packet/ConReqPacket.h"
#include "../Packet/MoveReqPacket.h"
#include "../Packet/ChatPacket.h"
#include "ctime"

#define BUFFERSIZE 512
#define PORT 9000


class ServerAgent
{
private:
	//For Socket
	USHORT SERVERPORT;
	WSADATA Wsadata;		//Initiate WinSock
	SOCKET Listen_Socket;
	SOCKADDR_IN ServerAddress;

	//Variable for Data Communication
	SOCKET clientSocket;
	vector<SOCKET> ClientSockets;
	SOCKADDR_IN clientAddress;
	INT AddressLen;
	TCHAR Buffer[BUFFERSIZE + 1];

public:
	ServerAgent();
	~ServerAgent();
	VOID error_Quit(TCHAR* Msg);
	VOID error_Display(TCHAR* Msg);
	VOID setReadyState();
	VOID communicate();
	static DWORD WINAPI SocketThread(LPVOID lpParam);
};