
#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

void ProcessHeaders(char*);
void ProcessQueryString(char*);
void ProcessPostBodyUrlEncoded(char*);

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

	char buf[2048];

	while (1)
	{
		SOCKET client = accept(listener, NULL, NULL);
		int ret = recv(client, buf, sizeof(buf), 0);
		if (ret <= 0)
		{
			closesocket(client);
			continue;
		}

		buf[ret] = 0;
		printf("%s\n", buf);

		// ProcessHeaders(buf);
		// ProcessQueryString(buf);

		ProcessPostBodyUrlEncoded(buf);

		closesocket(client);
	}
}


void ProcessHeaders(char* req)
{
	char* p1 = strstr(req, "\r\n");
	char* p2 = strstr(req, "\r\n\r\n");

	int headerLength = p2 - (p1 + 2);
	char* headers = (char*)malloc(headerLength + 1);
	memcpy(headers, p1 + 2, headerLength);
	headers[headerLength] = 0;

	// printf("Headers: \n%s", headers);

	char* header = strtok(headers, "\r\n");
	while (header != NULL)
	{
		// Xu ly header
		char* p = strstr(header, ": ");

		int nameLength = p - header;
		char* name = (char*)malloc(nameLength + 1);
		memcpy(name, header, nameLength);
		name[nameLength] = 0;

		int valueLength = strlen(header) - nameLength - 2;
		char* value = (char*)malloc(valueLength + 1);
		memcpy(value, p + 2, valueLength);
		value[valueLength] = 0;

		printf("Name: %s --- Value: %s\n", name, value);

		free(name);
		free(value);

		header = strtok(NULL, "\r\n");
	}

	free(headers);
}

void ProcessQueryString(char* req)
{
	char cmd[16], uri[256];
	sscanf(req, "%s %s", cmd, uri);

	char* p = strstr(uri, "?");
	if (p != NULL)
	{
		char* queryString = p + 1;
		char* param = strtok(queryString, "&");
		while (param != NULL)
		{
			// Xu ly header
			char* p = strstr(param, "=");

			int nameLength = p - param;
			char* name = (char*)malloc(nameLength + 1);
			memcpy(name, param, nameLength);
			name[nameLength] = 0;

			int valueLength = strlen(param) - nameLength - 1;
			char* value = (char*)malloc(valueLength + 1);
			memcpy(value, p + 1, valueLength);
			value[valueLength] = 0;

			printf("Name: %s --- Value: %s\n", name, value);

			free(name);
			free(value);

			param = strtok(NULL, "&");
		}
	}
	else
		printf("No query string!\n");
}

void ProcessPostBodyUrlEncoded(char* req)
{
	char* body = strstr(req, "\r\n\r\n") + 4;
	if (body != NULL)
	{
		char* param = strtok(body, "&");
		while (param != NULL)
		{
			// Xu ly header
			char* p = strstr(param, "=");

			int nameLength = p - param;
			char* name = (char*)malloc(nameLength + 1);
			memcpy(name, param, nameLength);
			name[nameLength] = 0;

			int valueLength = strlen(param) - nameLength - 1;
			char* value = (char*)malloc(valueLength + 1);
			memcpy(value, p + 1, valueLength);
			value[valueLength] = 0;

			printf("Name: %s --- Value: %s\n", name, value);

			free(name);
			free(value);

			param = strtok(NULL, "&");
		}
	}
	else
		printf("No query string!\n");
}