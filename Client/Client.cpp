#include "Client.h"

Client::Client()
{
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return;
}

Client::~Client()
{
    closesocket(sock);

    // ���� ����
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
    // ������ ������ ���
    while (true) {
        // ������ �Է�
  //      printf("\n[���� ������] ");
  //     
		//if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
		//	break;

		//// '\n' ���� ����
		//len = strlen(buf);
		//if (buf[len - 1] == '\n')
  //          buf[len - 1] = '\0';
		//if (strlen(buf) == 0)
		//	break;

        MoveReqPacket PK_DATA(EPacketHeader::PK_REQ_MOVE, 3, 4, 5);

        //// ������ ������
        PK_DATA.Serialize(buf);

        retval = send(sock, buf, PK_DATA.GetLen(), 0);
        if (retval == SOCKET_ERROR) {
            err_display((char*)"send()");
            break;
        }
        printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�.\n", retval);

        ////������ �ޱ�
        //retval = recv(sock, buf, retval, 0);
        //if (retval == SOCKET_ERROR) {
        //    err_display((char*)"recv()");
        //    break;
        //}
        //else if (retval == 0)
        //    break;

        //// ���� ������ ���
        //buf[retval] = '\0';
        //printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� �޾ҽ��ϴ�.\n", retval);
        //printf("[���� ������] %s\n", buf);
    }

}


int main(int argc, char* argv[])
{
    Client client;

    client.Init();
    client.Communication();

    return 0;

}