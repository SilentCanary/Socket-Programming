// #include <iostream>
// #include <winsock2.h>
// #include <thread>

// #pragma comment(lib, "ws2_32.lib")

// #define PORT 8080

// void receiveMessagesAndSend(SOCKET clientSocket) {
//     char buffer[1024];
//     while (true) {
//         // Receive messages from the server
//         memset(buffer, 0, sizeof(buffer));
//         int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
//         if (bytesReceived > 0) {
//             std::cout << "Server: " << buffer << std::endl;
//         }

//         // Send messages to the server
//         memset(buffer, 0, sizeof(buffer));
//         std::cin.getline(buffer, sizeof(buffer));
//         send(clientSocket, buffer, strlen(buffer), 0);
//     }
// }

// int main() {
//     WSADATA wsaData;
//     WSAStartup(MAKEWORD(2, 2), &wsaData);

//     SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);

//     sockaddr_in serverAddr;
//     serverAddr.sin_family = AF_INET;
//     serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Change to server IP if needed
//     serverAddr.sin_port = htons(PORT);

//     connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));

//     //std::thread(receiveMessages, clientSocket).detach();
//     std::thread(receiveMessagesAndSend, clientSocket).detach();
//     char buffer[1024];
//     while (true) {
//         std::cin.getline(buffer, sizeof(buffer));
//         send(clientSocket, buffer, strlen(buffer), 0);
//     }

//     closesocket(clientSocket);
//     WSACleanup();
//     return 0;
// }

// Client code

#include <iostream>
#include <winsock2.h>
#include <thread>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

#define PORT 8080

void receiveMessages(SOCKET clientSocket) {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived > 0) {
            std::cout << buffer << std::endl;
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

    // Send client's ID to the server
    std::string clientID = "Client 2"; // Change this to whatever client ID you want
    send(clientSocket, clientID.c_str(), clientID.size(), 0);

    std::thread(receiveMessages, clientSocket).detach();

    char buffer[1024];
    while (true) {
        std::cin.getline(buffer, sizeof(buffer));
        send(clientSocket, buffer, strlen(buffer), 0);
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}

