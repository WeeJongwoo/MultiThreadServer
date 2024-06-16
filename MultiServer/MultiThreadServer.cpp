#include "MultiTreadServer.h"
#include "../Packet/PacketHeader.h"

vector<SOCKET> ServerAgent::ClientSockets;
HANDLE ServerAgent::ServerMutex;

ServerAgent::ServerAgent()
{
	SERVERPORT = PORT;
	if (WSAStartup(MAKEWORD(2, 2), &Wsadata) != 0)
		return;

	ServerMutex = CreateMutex(NULL, FALSE, NULL);
}
ServerAgent::~ServerAgent()
{
	//CloseSocket()
	closesocket(Listen_Socket);

	//WIndsock Quit
	WSACleanup();
}
//Displaying Socket Error
VOID ServerAgent::error_Quit(TCHAR* Msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), (LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, Msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

//Displaying Socket Function Error
VOID ServerAgent::error_Display(TCHAR* Msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	std::wcout << "[" << Msg << "]" << (TCHAR*)lpMsgBuf;
	LocalFree(lpMsgBuf);
}

VOID ServerAgent::setReadyState()
{
	INT Retval;

	//Socket()
	Listen_Socket = socket(AF_INET, SOCK_STREAM, 0);
	if (Listen_Socket == INVALID_SOCKET)
		error_Quit((char*)("Socket()"));

	//bind()
	ZeroMemory(&ServerAddress, sizeof(SOCKADDR_IN));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	ServerAddress.sin_port = htons(SERVERPORT);
	Retval = bind(Listen_Socket, (SOCKADDR*)&ServerAddress, sizeof(ServerAddress));
	if (Retval == SOCKET_ERROR)
		error_Quit((char*)("Bind()"));

	//listen()
	Retval = listen(Listen_Socket, SOMAXCONN);
	if (Retval == SOCKET_ERROR)
		error_Quit((char*)("listen()"));
}

VOID ServerAgent::communicate()
{
	HANDLE hThread;

	while (1)
	{
		//Accept()
		AddressLen = sizeof(clientAddress);
		clientSocket = accept(Listen_Socket, (SOCKADDR*)&clientAddress, &AddressLen);
		if (clientSocket == INVALID_SOCKET)
		{
			error_Display((char*)("Accept"));
			break;
		}

		//Displaying Client Display
		std::wcout << std::endl << _T("[TCP Server] Client Connected : IP Address = ")
			<< inet_ntoa(clientAddress.sin_addr) << _T(", Port = ") << ntohs(clientAddress.sin_port) << std::endl;

		/*u_long on = 1;
		ioctlsocket(clientSocket, FIONBIO, &on);*/
		ClientSockets.push_back(clientSocket);

		//Create Thread
		hThread = CreateThread(NULL, 0, SocketThread, this, 0, NULL);
		if (hThread == NULL)
			closesocket(clientSocket);
		//else
			//CloseHandle(hThread);
	}
}

DWORD WINAPI ServerAgent::SocketThread(LPVOID lpParam)
{
	ServerAgent* This = (ServerAgent*)lpParam;
	SOCKET sock = This->clientSocket;

	INT retval;
	INT addressLen;
	SOCKADDR_IN threadSocketAddress;
	TCHAR Buffer[BUFFERSIZE + 1];
	TCHAR SnadBuffer[BUFFERSIZE + 1];

	//Get Client Information
	addressLen = sizeof(SOCKADDR_IN);
	getpeername(sock, (SOCKADDR*)&threadSocketAddress, &addressLen);

	while (1)
	{
		//Receving Data
		retval = recv(sock, Buffer, BUFFERSIZE, 0);
		if (retval == SOCKET_ERROR)
		{
			cout << GetLastError();
			break;
		}
		else if (retval == 0)
			break;

		EPacketHeader InPacketHeader;
		memset(&InPacketHeader, Buffer[PacketPos], sizeof(EPacketHeader));

		switch (InPacketHeader)
		{
		case EPacketHeader::PK_REQ_CON:
		{

			ConPacket RecvConPacket(InPacketHeader, new char[10]);
			RecvConPacket.Deserialize(Buffer);

			time_t timer = time(NULL);
			tm* time = localtime(&timer);

			cout << time->tm_year + 1900 << "년 " << time->tm_mon +1 << "월 " << time->tm_hour << "시 "
				<< time->tm_min << "분 " << time->tm_sec << "초, " 
				<< "Cilent ID: " << RecvConPacket.GetID() << " connected" << endl;

			WaitForSingleObject(ServerMutex, INFINITE);

			for (auto ClientSoc : ClientSockets)
			{
				RecvConPacket.Serialize(SnadBuffer);

				retval = send(ClientSoc, SnadBuffer, RecvConPacket.GetLen(), 0);
				if (retval == SOCKET_ERROR) {
					cout << "Sand Error";
					break;
				}
			}

			ReleaseMutex(ServerMutex);
			break;
		}
		case EPacketHeader::PK_REQ_MOVE:
		{
			MovePacket RecvMovePacket(InPacketHeader, "");
			RecvMovePacket.Deserialize(Buffer);

			cout << inet_ntoa(threadSocketAddress.sin_addr) << RecvMovePacket.GetID() <<" Move to: "
				<< " X " << RecvMovePacket.GetX() << " Y " << RecvMovePacket.GetY() << " Z " << RecvMovePacket.GetZ() << endl;

			for (auto ClientSoc : ClientSockets)
			{
				RecvMovePacket.Serialize(SnadBuffer);

				retval = send(ClientSoc, SnadBuffer, RecvMovePacket.GetLen(), 0);
				if (retval == SOCKET_ERROR) {
					cout << "Sand Error";
					break;
				}
			}

			break;
		}
		case EPacketHeader::PK_CHAT_STRING:
		{
			ChatPacket RecvChatPacket("", "");
			RecvChatPacket.Deserialize(Buffer);

			cout << RecvChatPacket.GetID() << ": "
				<< RecvChatPacket.GetChat() << endl;

			for (auto ClientSoc : ClientSockets)
			{
				RecvChatPacket.Serialize(SnadBuffer);

				retval = send(ClientSoc, SnadBuffer, RecvChatPacket.GetLen(), 0);
				if (retval == SOCKET_ERROR) {
					cout << "Sand Error";
					break;
				}
			}

			break;
		}
		case EPacketHeader::PK_EXIT:
		{
			ConPacket RecvConPacket(InPacketHeader, new char[10]);
			RecvConPacket.Deserialize(Buffer);

			time_t timer = time(NULL);
			tm* time = localtime(&timer);

			cout << time->tm_year + 1900 << "년 " << time->tm_mon + 1 << "월 " << time->tm_hour << "시 "
				<< time->tm_min << "분 " << time->tm_sec << "초, "
				<< "Cilent ID: " << RecvConPacket.GetID() << " disconnected" << endl;

			for (auto ClientSoc : ClientSockets)
			{
				RecvConPacket.Serialize(SnadBuffer);

				retval = send(ClientSoc, SnadBuffer, RecvConPacket.GetLen(), 0);
				if (retval == SOCKET_ERROR) {
					cout << "Sand Error";
					break;
				}
			}
			closesocket(sock);
		
			auto ClientSock = find(ClientSockets.begin(), ClientSockets.end(), sock);
			if (ClientSock != ClientSockets.end())
				ClientSockets.erase(ClientSock);

			std::wcout << _T("[TCP Server] Client Disconnected : IP Address=")
				<< inet_ntoa(threadSocketAddress.sin_addr) << _T("PORT = ") << ntohs(threadSocketAddress.sin_port) << std::endl;
			return 1;
		}
		default:
			break;
		}


		//Displaying Receiving Data
		//Buffer[retval+1] = _T('\0');
		/*std::wcout << _T("[TCP/") << inet_ntoa(threadSocketAddress.sin_addr)
			<< _T(":") << ntohs(threadSocketAddress.sin_port) << _T("  ") << Buffer << std::endl;*/


	}
	closesocket(sock);
	std::wcout << _T("[TCP Server] Client Disconnected : IP Address=")
		<< inet_ntoa(threadSocketAddress.sin_addr) << _T("PORT = ") << ntohs(threadSocketAddress.sin_port) << std::endl;

	return 1;
}

INT _tmain(INT argc, TCHAR* argv[])
{
	ServerAgent Server;
	Server.setReadyState();
	Server.communicate();

	return 0;
}