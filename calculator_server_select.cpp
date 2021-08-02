
#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

void ProcessData(char* buf, SOCKET client);

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

	SOCKET clients[64];
	int numClients = 0;

	int ret;
	char buf[256];

	while (1)
	{
		FD_ZERO(&fdread);
		
		FD_SET(listener, &fdread);
		for (int i = 0; i < numClients; i++)
			FD_SET(clients[i], &fdread);

		ret = select(0, &fdread, 0, 0, NULL);
		if (ret <= 0)
			break;

		if (FD_ISSET(listener, &fdread))
		{
			SOCKET client = accept(listener, NULL, NULL);
			printf("New client accepted: %d\n", client);

			clients[numClients] = client;
			numClients++;
		}

		for (int i = 0; i < numClients; i++)
			if (FD_ISSET(clients[i], &fdread))
			{
				ret = recv(clients[i], buf, sizeof(buf), 0);
				if (ret <= 0)
					continue;

				buf[ret] = 0;
				printf("Received: %s\n", buf);

				ProcessData(buf, clients[i]);
			}
	}
}

void ProcessData(char* buf, SOCKET client)
{
	float a, b, result;
	char cmd[16], tmp[64];
	int ret = sscanf(buf, "%s %f %f %s", cmd, &a, &b, tmp);
	if (ret == 3)
	{
		if (strcmp(cmd, "ADD") == 0)
		{
			result = a + b;
			sprintf(tmp, "OK %f\n", result);
			send(client, tmp, strlen(tmp), 0);
		}
		else if (strcmp(cmd, "SUB") == 0)
		{
			result = a - b;
			sprintf(tmp, "OK %f\n", result);
			send(client, tmp, strlen(tmp), 0);
		}
		else if (strcmp(cmd, "MUL") == 0)
		{
			result = a * b;
			sprintf(tmp, "OK %f\n", result);
			send(client, tmp, strlen(tmp), 0);
		}
		else if (strcmp(cmd, "DIV") == 0)
		{
			if (b != 0)
			{
				result = a / b;
				sprintf(tmp, "OK %f\n", result);
				send(client, tmp, strlen(tmp), 0);
			}
			else
			{
				const char* errMsg = "ERROR Phep chia cho 0.\n";
				send(client, errMsg, strlen(errMsg), 0);
			}
		}
		else
		{
			const char* errMsg = "ERROR Sai cu phap. Hay nhap lai.\n";
			send(client, errMsg, strlen(errMsg), 0);
		}
	}
	else
	{
		const char* errMsg = "ERROR Sai cu phap. Hay nhap lai.\n";
		send(client, errMsg, strlen(errMsg), 0);
	}
}