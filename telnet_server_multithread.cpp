// TelnetServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

DWORD WINAPI ClientThread(LPVOID);

int main()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(9000);

	SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	bind(listener, (SOCKADDR*)&addr, sizeof(addr));
	listen(listener, 5);

	while (1)
	{
		SOCKET client = accept(listener, NULL, NULL);
		CreateThread(0, 0, ClientThread, &client, 0, 0);
	}
}

DWORD WINAPI ClientThread(LPVOID lpParam)
{
	SOCKET client = *(SOCKET*)lpParam;

	char buf[256];
	int ret;

	char user[32], pass[32], tmp[32];
	char fbuf[64], fuser[32], fpass[32];

	// Thuc hien dang nhap
	while (1)
	{
		ret = recv(client, buf, sizeof(buf), 0);
		if (ret <= 0)
		{
			closesocket(client);
			return 0;
		}

		buf[ret] = 0;
		printf("Received: %s\n", buf);

		// Phan tich du lieu
		ret = sscanf(buf, "%s %s %s", user, pass, tmp);
		if (ret == 2)
		{
			// Doc du lieu tu file va so sanh 
			int found = 0;
			FILE* f = fopen("c:\\test\\users.txt", "r");
			while (!feof(f))
			{
				fgets(fbuf, sizeof(fbuf), f);
				ret = sscanf(fbuf, "%s %s", fuser, fpass);
				if (ret == 2)
				{
					if (strcmp(user, fuser) == 0 && strcmp(pass, fpass) == 0)
					{
						found = 1;
						break;
					}
				}
			}
			fclose(f);

			if (found == 1)
			{
				const char* okMsg = "Dang nhap thanh cong. Hay gui lenh de thuc hien.\n";
				send(client, okMsg, strlen(okMsg), 0);
				break;
			}
			else
			{
				const char* errorMsg = "Sai thong tin dang nhap. Hay nhap lai.\n";
				send(client, errorMsg, strlen(errorMsg), 0);
			}
		}
		else
		{
			const char* errorMsg = "Sai cu phap. Hay nhap lai.\n";
			send(client, errorMsg, strlen(errorMsg), 0);
		}
	}

	while (1)
	{
		ret = recv(client, buf, sizeof(buf), 0);
		if (ret <= 0)
		{
			closesocket(client);
			return 0;
		}

		if (buf[ret - 1] == '\n')
			buf[ret - 1] = 0;

		strcat(buf, " > c:\\test\\output.txt");
		ret = system(buf);

		FILE* f = fopen("c:\\test\\output.txt", "rb");
		while (!feof(f))
		{
			ret = fread(fbuf, 1, sizeof(fbuf), f);
			if (ret <= 0)
				break;
			send(client, fbuf, ret, 0);
		}
		fclose(f);
	}
}
