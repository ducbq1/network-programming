// ServerInfo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>

#include <winsock2.h>

#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

int main()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(9000);

	bind(listener, (SOCKADDR*)&addr, sizeof(addr));
	listen(listener, 5);

	SOCKET client = accept(listener, NULL, NULL);

	char buf[256];
	int ret = recv(client, buf, sizeof(buf), 0);
	if (ret <= 0)
		return 1;

	int n = 0;
	char computerName[64];

	sscanf(buf, "%s", computerName);
	printf("Computer Name: %s\n", computerName);
	n += strlen(computerName) + 1;

	int numDrives = (ret - n) / 5;
	for (int i = 0; i < numDrives; i++)
	{
		char driveLetter = buf[n];
		n++;

		float driveSize;
		memcpy(&driveSize, buf + n, sizeof(driveSize));
		n += sizeof(driveSize);

		printf("%c : %.0f GB\n", driveLetter, driveSize);
	}

	system("pause");

	closesocket(client);
	closesocket(listener);
	WSACleanup();

}
