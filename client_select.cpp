// ClientSelect.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>

#pragma comment(lib, "ws2_32")

int main()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(9000);

	SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	int ret = connect(client, (SOCKADDR*)&addr, sizeof(addr));

	fd_set fdread;

	char buf[256];

	struct timeval tv;
	tv.tv_sec = 3;
	tv.tv_usec = 0;

	while (1)
	{
		FD_ZERO(&fdread);
		FD_SET(client, &fdread);
		ret = select(0, &fdread, 0, 0, &tv);
		if (ret < 0)
			break;

		if (ret == 0)
		{
			printf("Timed out\n");
			continue;
		}

		ret = recv(client, buf, sizeof(buf), 0);
		if (ret <= 0)
			break;

		buf[ret] = 0;
		printf("Received: %s\n", buf);
	}

	closesocket(client);
	WSACleanup();
}
