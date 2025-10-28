#include "../NetworkDLL/Network.h"
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

std::mutex coutMutex;

void HandleClient(SOCKET clientSocket, int clientId) {
    try {
        NetworkSocket clientHandler;
        
        {
            std::lock_guard<std::mutex> lock(coutMutex);
            std::cout << "Client #" << clientId << " connected!" << std::endl;
        }
        
        std::string welcomeMsg = "Welcome to the server! You are client #" + std::to_string(clientId) + "\n";
        clientHandler.SendData(clientSocket, welcomeMsg);
        
        while (true) {
            std::string receivedData = clientHandler.ReceiveData(clientSocket, 1024);
            
            if (receivedData.empty()) {
                break;
            }
            
            {
                std::lock_guard<std::mutex> lock(coutMutex);
                std::cout << "Client #" << clientId << " says: " << receivedData << std::endl;
            }
            
            if (receivedData == "quit" || receivedData == "exit") {
                std::string byeMsg = "Goodbye client #" + std::to_string(clientId) + "!\n";
                clientHandler.SendData(clientSocket, byeMsg);
                break;
            }
            
            std::string response = "Server received: " + receivedData;
            clientHandler.SendData(clientSocket, response);
        }
        
        clientHandler.CloseSocket(clientSocket);
        
        {
            std::lock_guard<std::mutex> lock(coutMutex);
            std::cout << "Client #" << clientId << " disconnected." << std::endl;
        }
        
    } catch (const NetworkException& e) {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cerr << "Network exception for client #" << clientId << ": " << e.what() << std::endl;
    }
}

int main() {
    try {
        std::cout << "=== Server Starting ===" << std::endl;
        
        NetworkSocket serverSocket;
        
        if (!serverSocket.CreateSocket()) {
            std::cerr << "Failed to create server socket" << std::endl;
            return 1;
        }
        
        int port = 8080;
        if (!serverSocket.BindSocket(port)) {
            std::cerr << "Failed to bind to port " << port << std::endl;
            return 1;
        }
        
        if (!serverSocket.ListenSocket(5)) {
            std::cerr << "Failed to listen on socket" << std::endl;
            return 1;
        }
        
        std::cout << "Server listening on port " << port << "..." << std::endl;
        std::cout << "Waiting for clients to connect..." << std::endl;
        
        int clientId = 0;
        std::vector<std::thread> clientThreads;
        
        while (true) {
            SOCKET clientSocket = serverSocket.AcceptConnection();
            
            if (clientSocket != INVALID_SOCKET) {
                clientId++;
                clientThreads.emplace_back(HandleClient, clientSocket, clientId);
                clientThreads.back().detach();
            }
        }
        
    } catch (const NetworkException& e) {
        std::cerr << "Network exception: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
