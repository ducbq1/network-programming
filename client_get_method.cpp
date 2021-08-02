// Client2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32")

int main()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Phan giai ten mien
    addrinfo* info;
    SOCKADDR_IN addr;
    int ret = getaddrinfo("bunoc.net", "http", NULL, &info);
    if (ret != 0)
    {
        printf("getaddrinfo() failed\n");
        return 1;
    }

    memcpy(&addr, info->ai_addr, info->ai_addrlen);

    connect(client, (SOCKADDR*)&addr, sizeof(addr));

    char buf[2048] = "GET / HTTP/1.1\r\nHost: bunoc.net\r\n\r\n";
    send(client, buf, strlen(buf), 0);

    while (1)
    {
        ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
            break;

        if (ret < sizeof(buf))
            buf[ret] = 0;

        printf("%s", buf);
    }

    closesocket(client);
    WSACleanup();
}