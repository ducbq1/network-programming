
#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

DWORD WINAPI ClientThread(LPVOID);
void ProcessData(char* buf, SOCKET client);

int main()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(9090);

	SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	bind(listener, (SOCKADDR*)&addr, sizeof(addr));
	listen(listener, 5);

	while (1)
	{
		SOCKET client = accept(listener, NULL, NULL);
		printf("New client accepted: %d\n", client);
		CreateThread(0, 0, ClientThread, &client, 0, 0);
	}
}

DWORD WINAPI ClientThread(LPVOID lpParam)
{
	SOCKET client = *(SOCKET*)lpParam;
	char buf[256];
	int ret;

	while (1)
	{
		ret = recv(client, buf, sizeof(buf), 0);
		if (ret <= 0)
			break;

		buf[ret] = 0;
		printf("Received: %s\n", buf);

		ProcessData(buf, client);
	}
}

void ProcessData(char* buf, SOCKET client)
{
	float a, b, result;
	char cmd[16], tmp[64];
	int ret = sscanf(buf, "%f %s %f %s", &a, cmd, &b, tmp);
	if (ret == 3)
	{
		if (strcmp(cmd, "+") == 0)
		{
			result = a + b;
			sprintf(tmp, "OK %f\n", result);
			send(client, tmp, strlen(tmp), 0);
		} 
		else if (strcmp(cmd, "-") == 0)
		{
			result = a - b;
			sprintf(tmp, "OK %f\n", result);
			send(client, tmp, strlen(tmp), 0);
		}
		else if (strcmp(cmd, "*") == 0)
		{
			result = a * b;
			sprintf(tmp, "OK %f\n", result);
			send(client, tmp, strlen(tmp), 0);
		}
		else if (strcmp(cmd, "/") == 0)
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