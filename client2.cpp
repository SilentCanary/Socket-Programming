
#include <iostream>
#include <winsock2.h>
#include <thread>
#include <string>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

#define PORT 8080

void receiveMessages(SOCKET clientSocket) {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived > 0) {
            cout << buffer << std::endl;
        }
    }
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(PORT);

    connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));

    string clientID = "Client 2"; 
    send(clientSocket, clientID.c_str(), clientID.size(), 0);

    thread(receiveMessages, clientSocket).detach(); // detaching here basically meant that reccieve mesage fxn would continue to be called and main fxning can continue.. it doesn't wait for thread to over hencewhile we are tyoing text for this client if someone sends a msg it can recieve


    char buffer[1024];
    while (true) {
        cin.getline(buffer, sizeof(buffer));
        send(clientSocket, buffer, strlen(buffer), 0);
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
