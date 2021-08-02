// ClientInfo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock.h>

#pragma comment(lib, "ws2_32")

int main()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(9000);

	system("pause");
	int ret = connect(client, (SOCKADDR*)&addr, sizeof(addr));

	char buf[256];
	int n = 0;

	char computerName[64];
	DWORD computerNameLength;
	GetComputerNameA(computerName, &computerNameLength);

	memcpy(buf, computerName, computerNameLength + 1);
	n += computerNameLength + 1;

	char driveString[64];
	DWORD sectorsPerCluster, bytesPerSector, numberOfFreeClusters, totalNumberOfClusters;

	ret = GetLogicalDriveStringsA(sizeof(driveString), driveString);
	for (int i = 0; i < ret / 4; i++)
	{
		buf[n] = driveString[i * 4];
		n++;

		GetDiskFreeSpaceA(&driveString[i * 4], &sectorsPerCluster, &bytesPerSector, &numberOfFreeClusters, &totalNumberOfClusters);
		// double freeBytes = (double)numberOfFreeClusters * (double)sectorsPerCluster * (double)bytesPerSector / (double)(1 << 30);
		// double totalBytes = (double)totalNumberOfClusters * (double)sectorsPerCluster * (double)bytesPerSector / (double)(1 << 30);

		float totalBytes = (float)totalNumberOfClusters * (float)sectorsPerCluster * (float)bytesPerSector / (float)(1 << 30);
		memcpy(buf + n, &totalBytes, sizeof(totalBytes));
		n += sizeof(totalBytes);
	}
	
	send(client, buf, n, 0);
	closesocket(client);
	WSACleanup();
}
