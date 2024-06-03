#include <iostream>
#include <winsock2.h>
#include <thread>
#include <vector>
#include <mutex>
#include <algorithm>
#include <string>
#include <map>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

#define PORT 8080

vector<SOCKET> clients;
mutex clientsMutex;
map<SOCKET, string> clientIDs; // To store client IDs

void handleClient(SOCKET clientSocket)
{
    char buffer[1024];
    string clientID = clientIDs[clientSocket]; // Get client ID
    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0)
        {
            {
                clientsMutex.lock();
                auto it = std::remove(clients.begin(), clients.end(), clientSocket);
                clients.erase(it, clients.end());
                clientsMutex.unlock();
            }
            closesocket(clientSocket);
            break;
        }
        cout << clientID << ": " << buffer << std::endl;

        {
            clientsMutex.lock();
            for (auto client : clients)
            {
                if (client != clientSocket)
                {
                    send(client, (clientID + ": " + buffer).c_str(), bytesReceived + clientID.size() + 2, 0);
                }
            }
            clientsMutex.unlock();
        }
    }
}

void sendToAllClients(string &message)
{
    clientsMutex.lock();
    for (auto client : clients)
    {
        send(client, message.c_str(), message.size(), 0);
    }
    clientsMutex.unlock();
}

void userInputThread()
{
    string userInput;
    while (true)
    {
        getline(cin, userInput);
        sendToAllClients("Server: " + userInput);
    }
}

int main()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    bind(serverSocket, (sockaddr *)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, SOMAXCONN);

    cout << "Server is listening on port " << PORT << endl;

    thread inputThread(userInputThread);

    while (true)
    {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        {
            clientsMutex.lock();
            clients.push_back(clientSocket);
            clientsMutex.unlock();
        }
        char clientIDBuffer[1024];
        int bytesReceived = recv(clientSocket, clientIDBuffer, sizeof(clientIDBuffer), 0);
        if (bytesReceived > 0)
        {
            clientIDBuffer[bytesReceived] = '\0';
            clientIDs[clientSocket] = string(clientIDBuffer);
        }
        // Handle each client in a separate thread
        thread(handleClient, clientSocket).detach();
    }

    inputThread.join();

    WSACleanup();
    return 0;
}
