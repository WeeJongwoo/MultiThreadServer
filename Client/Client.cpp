#include "Client.h"

Client::Client()
{
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return;
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
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) err_quit((char*)"socket()");

    // connect()
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit((char*)"connect()");
}

void Client::Communication()
{
    char buf[BUFSIZE + 1];
    //int len;
    // 서버와 데이터 통신
    while (true) {
        // 데이터 입력
  //      printf("\n[보낼 데이터] ");
  //     
		//if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
		//	break;

		//// '\n' 문자 제거
		//len = strlen(buf);
		//if (buf[len - 1] == '\n')
  //          buf[len - 1] = '\0';
		//if (strlen(buf) == 0)
		//	break;

        MoveReqPacket PK_DATA(EPacketHeader::PK_REQ_MOVE, 3, 4, 5);

        //// 데이터 보내기
        PK_DATA.Serialize(buf);

        retval = send(sock, buf, PK_DATA.GetLen(), 0);
        if (retval == SOCKET_ERROR) {
            err_display((char*)"send()");
            break;
        }
        printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);

        ////데이터 받기
        //retval = recv(sock, buf, retval, 0);
        //if (retval == SOCKET_ERROR) {
        //    err_display((char*)"recv()");
        //    break;
        //}
        //else if (retval == 0)
        //    break;

        //// 받은 데이터 출력
        //buf[retval] = '\0';
        //printf("[TCP 클라이언트] %d바이트를 받았습니다.\n", retval);
        //printf("[받은 데이터] %s\n", buf);
    }

}


int main(int argc, char* argv[])
{
    Client client;

    client.Init();
    client.Communication();

    return 0;

}