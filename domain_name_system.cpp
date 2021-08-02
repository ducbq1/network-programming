#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

int main() {

    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);


    addrinfo* info;
    SOCKADDR_IN addr;

    char* domainName = new char[50];
    cout << "Nhap vao ten mien: ";
    cin >> domainName;
    int length = strlen(domainName);
    for (int i = 0; i < length; i++) {
        if (!(domainName[i] >= 'a' && domainName[i] <= 'z'
            || domainName[i] >= 'A' && domainName[i] <= 'Z'
            || domainName[i] == '.'
            || domainName[i] == '_'
            )) {
            cout << "Khong phai ten mien";
            return 0;
        }
    }

    int ret = getaddrinfo(domainName, "http", NULL, &info);
    if (ret == 0) {
        memcpy(&addr, info->ai_addr, info->ai_addrlen);
        cout << "Phan giai ten mien thanh cong: " << gethostbyname(domainName) << endl;
        cout << "Dia chi IP: " << inet_ntoa(addr.sin_addr);
    } else {
        cout << "Khong phan giai duoc ten mien\n";
    }
    delete[]domainName;
    return 0; 

}