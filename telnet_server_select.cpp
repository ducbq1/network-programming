// TelnetServerSelect.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

typedef struct {
	SOCKET client;
	BOOL isRegistered;
} CLIENT_INFO;

void RemoveClient(CLIENT_INFO*, int*, int*);

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

	fd_set fdread;

	CLIENT_INFO clients[64];
	int numClients = 0;

	int ret;
	char buf[256];

	char user[32], pass[32], tmp[32];
	char fbuf[64], fuser[32], fpass[32];

	while (1)
	{
		FD_ZERO(&fdread);

		FD_SET(listener, &fdread);
		for (int i = 0; i < numClients; i++)
			FD_SET(clients[i].client, &fdread);

		ret = select(0, &fdread, 0, 0, 0);
		if (ret < 0)
			break;

		if (FD_ISSET(listener, &fdread))
		{
			SOCKET client = accept(listener, NULL, NULL);
			printf("New client accepted: %d\n", client);

			CLIENT_INFO clientInfo;
			clientInfo.client = client;
			clientInfo.isRegistered = FALSE;

			clients[numClients] = clientInfo;
			numClients++;
		}

		for (int i = 0; i < numClients; i++)
			if (FD_ISSET(clients[i].client, &fdread))
			{
				ret = recv(clients[i].client, buf, sizeof(buf), 0);
				if (ret <= 0)
				{
					RemoveClient(clients, &numClients, &i);
					continue;
				}

				buf[ret] = 0;
				printf("Received: %s\n", buf);

				// Kiem tra trang thai dang nhap cua client
				if (clients[i].isRegistered == FALSE)
				{
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
							send(clients[i].client, okMsg, strlen(okMsg), 0);
							clients[i].isRegistered = TRUE;
						}
						else
						{
							const char* errorMsg = "Sai thong tin dang nhap. Hay nhap lai.\n";
							send(clients[i].client, errorMsg, strlen(errorMsg), 0);
						}
					}
					else
					{
						const char* errorMsg = "Sai cu phap. Hay nhap lai.\n";
						send(clients[i].client, errorMsg, strlen(errorMsg), 0);
					}
				}
				else
				{
					// Thuc hien lenh
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
						send(clients[i].client, fbuf, ret, 0);
					}
					fclose(f);
				}
			}
	}

}

void RemoveClient(CLIENT_INFO* clients, int* numClients, int* i)
{
	if (*i < *numClients - 1)
		clients[*i] = clients[*numClients - 1];
	*numClients = *numClients - 1;
	*i = *i - 1;
}