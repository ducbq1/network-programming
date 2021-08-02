
#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)




int main() {

    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9000);

    SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bind(listener, (SOCKADDR*)&addr, sizeof(addr));
    listen(listener, 5);

    fd_set master;
    FD_ZERO(&master);
    FD_SET(listener, &master);

    bool running = true;
    while (running) {

        printf("Dang cho cac ket noi...\n");
        fd_set copy = master;
        int socketCount = select(0, &copy, nullptr, nullptr, nullptr);


        for (int i = 0; i < socketCount; i++) {
            SOCKET sock = copy.fd_array[i];

            if (sock == listener) {

                SOCKET client = accept(listener, nullptr, nullptr);

                const char* helloMsg = "Dang nhap theo cu phap \"[client_id:] [your_id]\".\n";
                send(client, helloMsg, strlen(helloMsg), 0);

                char buf[4096];
                char cmd[16], id[32], tmp[32];

                while (1) {

                    int ret = recv(client, buf, sizeof(buf), 0);
                    const char* errorMsg = "Sai cu phap. Hay nhap lai.\n";
                    const char* okMsg = "Welcome to the Awesome Chat Server!\r\n";

                    if (ret <= 0) {
                        closesocket(client);
                        return 0;
                    }
                    else {
                        buf[ret] = 0;
                        printf("Received: %s\n", buf);
                    }


                    ret = sscanf(buf, "%s %s %s", cmd, id, tmp);

                    if (ret == 2) {
                        if (strcmp(cmd, "client_id:") == 0) {
                            FD_SET(client, &master);
                            send(client, okMsg, strlen(okMsg), 0);
                            printf("Ket noi moi: %d\n\n", client);
                            break;
                        }
                        else {
                            send(client, errorMsg, strlen(errorMsg), 0);
                        }
                    }
                    else {
                        send(client, errorMsg, strlen(errorMsg), 0);
                    }
                }


            }
            else {

                char buf[4096];
                char sendbuf[4096];
                ZeroMemory(buf, 4096);
                int bytesIn = recv(sock, buf, 4096, 0);
                if (bytesIn <= 0) {
                    closesocket(sock);
                    FD_CLR(sock, &master);
                } else {
                  if (buf[0] == '\\') {
                    string cmd = string(buf, bytesIn);
                    if (cmd == "\\quit") {
                      running = false;
                      break;
                    }
                    continue;
                  }
                }
                for (int i = 0; i < master.fd_count; i++) {
                    SOCKET outSock = master.fd_array[i];
                    if (outSock != listener && outSock != sock) {
                        printf("SOCKET #%d: %s", sock, buf);
                        sprintf(sendbuf, "SOCKET #%d: %s\r\n", sock, buf);
                        send(outSock, sendbuf, strlen(sendbuf), 0);
                        // ostringsttream ss;
                        // ss << "SOCKET #" << sock << ": " << buf << "\r\r";
                        // string strOut = ss.str();
                        // send(outSock, strOut.c_str(), strOut.size() + 1, 0);
                    }
                }

            }
        }

    }

    FD_CLR(listener, &master);
    closesocket(listener);
    
    string msg = "Server is shutting down. Goodbye\r\n";

    while (master.fd_count > 0) {
      SOCKET sock = master.fd_array[0];
      send(sock, msg.c_str(), msg.size() + 1, 0);
      FD_CLR(sock, &master);
      closesocket(sock);
    }

    WSACleanup();
    system("pause");
    return 1;
}


