#include "../NetworkDLL/Network.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

int main() {
    try {
        std::cout << "=== Client Starting ===" << std::endl;
        
        NetworkSocket clientSocket;
        
        if (!clientSocket.CreateSocket()) {
            std::cerr << "Failed to create client socket" << std::endl;
            return 1;
        }
        
        std::string serverIP = "127.0.0.1";
        int serverPort = 8080;
        
        std::cout << "Connecting to server " << serverIP << ":" << serverPort << "..." << std::endl;
        
        if (!clientSocket.ConnectToServer(serverIP, serverPort)) {
            std::cerr << "Failed to connect to server" << std::endl;
            return 1;
        }
        
        std::cout << "Connected to server successfully!" << std::endl;
        
        std::string welcomeMsg = clientSocket.ReceiveData(1024);
        if (!welcomeMsg.empty()) {
            std::cout << "Server: " << welcomeMsg << std::endl;
        }
        
        std::cout << "\nYou can now send messages to the server." << std::endl;
        std::cout << "Type 'quit' or 'exit' to disconnect.\n" << std::endl;
        
        std::string message;
        while (true) {
            std::cout << "Enter message: ";
            std::getline(std::cin, message);
            
            if (message.empty()) {
                continue;
            }
            
            int bytesSent = clientSocket.SendData(message);
            if (bytesSent <= 0) {
                std::cerr << "Failed to send message" << std::endl;
                break;
            }
            
            std::string response = clientSocket.ReceiveData(1024);
            if (response.empty()) {
                std::cout << "Server disconnected." << std::endl;
                break;
            }
            
            std::cout << "Server response: " << response << std::endl;
            
            if (message == "quit" || message == "exit") {
                std::cout << "Disconnecting from server..." << std::endl;
                break;
            }
        }
        
        clientSocket.CloseSocket();
        std::cout << "Client disconnected." << std::endl;
        
    } catch (const NetworkException& e) {
        std::cerr << "Network exception: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
