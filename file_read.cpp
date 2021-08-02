// FileClient.cpp
//

#include <stdio.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>

#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

int main()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(8000);

    system("pause");
    int ret = connect(client, (SOCKADDR*)&addr, sizeof(addr));
    if (ret == SOCKET_ERROR)
    {
        ret = WSAGetLastError();
        printf("connect() failed: error code %d\n", ret);
        return 1;
    }

    // Doc du lieu tu file va gui sang server
    FILE* f = fopen("C:\\Test_client\\test.jpg", "rb");
    char buf[2048];

    printf("Sending: ");
    int sum = 0;

    while (1)
    {
        ret = fread(buf, 1, sizeof(buf), f);
        if (ret <= 0)
            break;

        send(client, buf, ret, 0);
        
        sum += ret;
        printf("\r%d", sum);
    }

    printf("\nFinished");

    fclose(f);

    closesocket(client);
    WSACleanup();
}
