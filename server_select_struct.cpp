// ChatServerSelect.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

typedef struct {
	SOCKET client;
	BOOL isRegistered;
	char* id;
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

	char cmd[16], id[32], tmp[32];
	const char* errorMsg = "Sai cu phap. Hay nhap lai.\n";
	char sendBuf[256];

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
			clientInfo.id = NULL;

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
					// Chua dang nhap, hoi client_id
					// Kiem tra cu phap
					ret = sscanf(buf, "%s %s %s", cmd, id, tmp);
					if (ret == 2)
					{
						if (strcmp(cmd, "client_id:") == 0)
						{
							const char* okMsg = "Dang nhap thanh cong. Hay nhap thong diep de chuyen tiep.\n";
							send(clients[i].client, okMsg, strlen(okMsg), 0);

							clients[i].id = (char *)malloc(strlen(id) + 1);
							memcpy(clients[i].id, id, strlen(id) + 1);
							clients[i].isRegistered = TRUE;
						}
						else
							send(clients[i].client, errorMsg, strlen(errorMsg), 0);
					}
					else
						send(clients[i].client, errorMsg, strlen(errorMsg), 0);
				}
				else
				{
					// Chuyen tiep tin nhan
					ret = sscanf(buf, "%s", cmd);

					if (ret == -1 || cmd[0] != '@')
					{
						send(clients[i].client, errorMsg, strlen(errorMsg), 0);
					}
					else
					{
						sprintf(sendBuf, "%s: %s", id, buf + strlen(cmd) + 1);

						if (strcmp(cmd, "@all") == 0)
						{
							// Chuyen tiep tin nhan den cac client khac
							for (int j = 0; j < numClients; j++)
								if (clients[j].isRegistered && clients[j].client != clients[i].client)
										send(clients[j].client, sendBuf, strlen(sendBuf), 0);
						}
						else
						{
							for (int j = 0; j < numClients; j++)
								if (clients[j].isRegistered && strcmp(clients[j].id, cmd + 1) == 0)
									send(clients[j].client, sendBuf, strlen(sendBuf), 0);
						}
					}
				}
			}
	}

}

void RemoveClient(CLIENT_INFO* clients, int* numClients, int* i) 
{
	if (*i < *numClients - 1)
		clients[*i] = clients[*numClients - 1];
	free(clients[*i].id);
	*numClients = *numClients - 1;
	*i = *i - 1;
}