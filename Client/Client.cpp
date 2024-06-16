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

    cout << "Init ID:  ";
    cin >> ID;

    ConReqPacket ConPAcket(EPacketHeader::PK_REQ_CON, ID);
    ConPAcket.Serialize(buf);

    retval = send(sock, buf, ConPAcket.GetLen(), 0);
    if (retval == SOCKET_ERROR) {
        err_display((char*)"send()");
        return;
    }
    printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�.\n", retval);

    //int len;
    // ������ ������ ���
    while (true) {
        char Select;

        cout << "�ൿ�� �����ϼ���. " << "'M': �̵� / 'C': ä�� / 'E': ����" << endl;
        cin >> Select;

        cin.ignore(1, ' ');

        switch (Select)
        {
        case 'm':
        case 'M':
        {
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');
            int  X ,Y, Z;
            cout << "�̵��� ��ǥ�� �Է����ּ���(����: 2 3 4): ";
            cin >> X >> Y >> Z;

            MoveReqPacket ReqMovePacket(EPacketHeader::PK_REQ_MOVE, X, Y, Z);

            //// ������ ������
            ReqMovePacket.Serialize(buf);

            retval = send(sock, buf, ReqMovePacket.GetLen(), 0);
            if (retval == SOCKET_ERROR) {
                err_display((char*)"send()");
                break;
            }
            printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�.\n", retval);

            break;
        }
        case 'c':
        case 'C':
        {
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');
            string Chat;

            cout << "ä���� �Է��ϼ���: ";
            getline(cin, Chat);

            ChatPacket ReqChatPcaket(Chat.c_str());
            ReqChatPcaket.Serialize(buf);

            retval = send(sock, buf, ReqChatPcaket.GetLen(), 0);
            if (retval == SOCKET_ERROR) {
                err_display((char*)"send()");
                break;
            }
            printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�.\n", retval);

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
            printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�.\n", retval);
            return;
        }

        default:
        {
            cout << "�߸��� �ൿ�Դϴ�. �ٽ� �Է����ּ���." << endl;
            break;
        }
        }


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