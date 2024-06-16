#include "Client.h"

HANDLE Client::Mutex;

Client::Client()
{

}

Client::~Client()
{
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
}

void Client::err_quit(char* msg)
{
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void Client::err_display(char* msg)
{
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void Client::Init()
{
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;

	Mutex = CreateMutex(NULL, FALSE, NULL);

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit((char*)"socket()");

	// connect()
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit((char*)"connect()");

	/*u_long on = 1;
	ioctlsocket(sock, FIONBIO, &on);*/
}

void Client::Communication()
{
	char buf[BUFSIZE + 1];

	cout << "Init ID:  ";
	cin >> ID;

	ConPacket ConPacket(EPacketHeader::PK_REQ_CON, ID.c_str());
	ConPacket.Serialize(buf);

	retval = send(sock, buf, ConPacket.GetLen(), 0);
	if (retval == SOCKET_ERROR) {
		err_display((char*)"send()");
		return;
	}
	printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);

	HANDLE Thread[2];
	Thread[0] = CreateThread(NULL, 0, ProcRecv, this, 0, NULL);
	Thread[1] = CreateThread(NULL, 0, ProcSend, this, 0, NULL);

	WaitForMultipleObjects(2, Thread, TRUE, INFINITE);

	if (Thread[0] == NULL)
		CloseHandle(Thread[0]);
	if (Thread[1] == NULL)
		CloseHandle(Thread[1]);

}


DWORD WINAPI Client::ProcSend(LPVOID lpParam)
{


	char buf[BUFSIZE + 1];

	Client* This = (Client*)lpParam;
	SOCKET sock = This->sock;
	int retval = This->retval;

	while (true) {
		char Select;

		cout << "행동을 선택하세요. " << "'M': 이동 / 'C': 채팅 / 'E': 종료" << endl;
		cin >> Select;
		cin.ignore(1, ' ');

		switch (Select)
		{
		case 'm':
		case 'M':
		{
			if (cin.gcount() >= 2)
				cin.ignore((numeric_limits<streamsize>::max)(), '\n');

			int  X, Y, Z;
			cout << "이동할 좌표를 입력해주세요(예시: 2 3 4): ";
			cin >> X >> Y >> Z;

			MovePacket ReqMovePacket(EPacketHeader::PK_REQ_MOVE, This->ID.c_str(), X, Y, Z);

			//// 데이터 보내기
			ReqMovePacket.Serialize(buf);

			retval = send(sock, buf, ReqMovePacket.GetLen(), 0);
			if (retval == SOCKET_ERROR) {
				This->err_display((char*)"send()");
				break;
			}
			printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);
			break;
		}
		case 'c':
		case 'C':
		{
			if (cin.gcount() >= 2)
				cin.ignore((numeric_limits<streamsize>::max)(), '\n');

			string Chat;

			cout << "채팅을 입력하세요: ";
			getline(cin, Chat);

			ChatPacket ReqChatPcaket(Chat.c_str(), This->ID.c_str());
			ReqChatPcaket.Serialize(buf);

			retval = send(sock, buf, ReqChatPcaket.GetLen(), 0);
			if (retval == SOCKET_ERROR) {
				This->err_display((char*)"send()");
				break;
			}
			printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);
			break;
		}
		case 'e':
		case 'E':
		{
			if (cin.gcount() >= 2)
				cin.ignore((numeric_limits<streamsize>::max)(), '\n');

			ConPacket ConPAcket(EPacketHeader::PK_EXIT, This->ID.c_str());
			ConPAcket.Serialize(buf);

			retval = send(sock, buf, ConPAcket.GetLen(), 0);
			if (retval == SOCKET_ERROR) {
				This->err_display((char*)"send()");
				break;
			}
			printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);
			return 1;
		}

		default:
		{
			cout << "잘못된 행동입니다. 다시 입력해주세요." << endl;
			break;
		}
		}
		
	}
}

DWORD WINAPI Client::ProcRecv(LPVOID lpParam)
{
	while (true)
	{

		char buf[BUFSIZE + 1];

		Client* This = (Client*)lpParam;
		SOCKET sock = This->sock;
		int retval = This->retval;

		retval = recv(sock, buf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR) {
			This->err_display((char*)"recv()");
		}
		else if (retval == 0)
			break;

		WaitForSingleObject(Mutex, INFINITE);

		EPacketHeader InPacketHeader;
		memset(&InPacketHeader, buf[PacketPos], sizeof(EPacketHeader));

		switch (InPacketHeader)
		{
		case EPacketHeader::PK_REQ_CON:
		{

			ConPacket RecvConPacket(InPacketHeader, "");
			RecvConPacket.Deserialize(buf);

			time_t timer = time(NULL);
			tm* time = localtime(&timer);

			cout << time->tm_year + 1900 << "년 " << time->tm_mon + 1 << "월 " << time->tm_hour << "시 "
				<< time->tm_min << "분 " << time->tm_sec << "초, "
				<< "Cilent ID: " << RecvConPacket.GetID() << " connected" << endl;

			break;
		}
		case EPacketHeader::PK_REQ_MOVE:
		{
			MovePacket RecvMovePacket(InPacketHeader, "");
			RecvMovePacket.Deserialize(buf);

			cout << " " << RecvMovePacket.GetID() << " Move to: "
				<< " X " << RecvMovePacket.GetX() << " Y " << RecvMovePacket.GetY() << " Z " << RecvMovePacket.GetZ() << endl;

			break;
		}
		case EPacketHeader::PK_CHAT_STRING:
		{
			ChatPacket RecvChatPacket("", "");
			RecvChatPacket.Deserialize(buf);

			cout << " " << RecvChatPacket.GetID() << ": "
				<< RecvChatPacket.GetChat() << endl;

			break;
		}
		case EPacketHeader::PK_EXIT:
		{
			ConPacket RecvConPacket(InPacketHeader, "");
			RecvConPacket.Deserialize(buf);

			time_t timer = time(NULL);
			tm* time = localtime(&timer);

			cout << time->tm_year + 1900 << "년 " << time->tm_mon + 1 << "월 " << time->tm_hour << "시 "
				<< time->tm_min << "분 " << time->tm_sec << "초, "
				<< "Cilent ID: " << RecvConPacket.GetID() << " disconnected" << endl;
			ReleaseMutex(Mutex);
			return 1;
		}
		default:
			break;
		}
		ReleaseMutex(Mutex);
	}
	return 1;
}


int main(int argc, char* argv[])
{
	Client client;

	client.Init();
	client.Communication();

	return 0;

}