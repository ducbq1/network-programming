
#include <stdio.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>

#pragma comment(lib, "ws2_32")

DWORD WINAPI ReceiverThread(LPVOID);

SOCKET client;

int main()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(9000);

	client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	int ret = connect(client, (SOCKADDR*)&addr, sizeof(addr));

	CreateThread(0, 0, ReceiverThread, 0, 0, 0);

	char buf[256];

	while (1)
	{
		// Doc du lieu tu ban phim va gui len server
		printf("Nhap du lieu: ");
		gets_s(buf);

		send(client, buf, strlen(buf), 0);
	}

	closesocket(client);
	WSACleanup();
}

DWORD WINAPI ReceiverThread(LPVOID lpParam)
{
	int ret;
	char buf[256];

	while (1)
	{
		// Nhan du lieu tu server
		ret = recv(client, buf, sizeof(buf), 0);
		if (ret <= 0)
			break;

		buf[ret] = 0;
		printf("Received: %s\n", buf);
	}
}