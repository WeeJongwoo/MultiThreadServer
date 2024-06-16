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

    cout << "Init ID:  ";
    cin >> ID;

    ConReqPacket ConPAcket(EPacketHeader::PK_REQ_CON, ID);
    ConPAcket.Serialize(buf);

    retval = send(sock, buf, ConPAcket.GetLen(), 0);
    if (retval == SOCKET_ERROR) {
        err_display((char*)"send()");
        return;
    }
    printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);

    //int len;
    // 서버와 데이터 통신
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
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');
            int  X ,Y, Z;
            cout << "이동할 좌표를 입력해주세요(예시: 2 3 4): ";
            cin >> X >> Y >> Z;

            MoveReqPacket ReqMovePacket(EPacketHeader::PK_REQ_MOVE, X, Y, Z);

            //// 데이터 보내기
            ReqMovePacket.Serialize(buf);

            retval = send(sock, buf, ReqMovePacket.GetLen(), 0);
            if (retval == SOCKET_ERROR) {
                err_display((char*)"send()");
                break;
            }
            printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);

            break;
        }
        case 'c':
        case 'C':
        {
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');
            string Chat;

            cout << "채팅을 입력하세요: ";
            getline(cin, Chat);

            ChatPacket ReqChatPcaket(Chat.c_str());
            ReqChatPcaket.Serialize(buf);

            retval = send(sock, buf, ReqChatPcaket.GetLen(), 0);
            if (retval == SOCKET_ERROR) {
                err_display((char*)"send()");
                break;
            }
            printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);

            break;
        }
        case 'e':
        case 'E':
        {
            ConReqPacket ConPAcket(EPacketHeader::PK_EXIT, ID);
            ConPAcket.Serialize(buf);

            retval = send(sock, buf, ConPAcket.GetLen(), 0);
            if (retval == SOCKET_ERROR) {
                err_display((char*)"send()");
                return;
            }
            printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);
            return;
        }

        default:
        {
            cout << "잘못된 행동입니다. 다시 입력해주세요." << endl;
            break;
        }
        }


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