// Server1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>

#include <winsock2.h>

#pragma comment(lib, "ws2_32")

int main()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(8000);

    bind(listener, (SOCKADDR*)&addr, sizeof(addr));
    listen(listener, 5);

    SOCKET client = accept(listener, NULL, NULL);

    char buf[256];
    int ret = recv(client, buf, sizeof(buf), 0);
    if (ret <= 0)
    {
        printf("recv() failed\n");
        return 1;
    }

    buf[ret] = 0;
    printf("Received: %s\n", buf);

    while (1)
    {
        printf("Enter data: ");
        gets_s(buf);

        if (strcmp(buf, "exit") == 0)
            break;

        send(client, buf, strlen(buf), 0);
    }

    closesocket(client);
    closesocket(listener);
    WSACleanup();
}
