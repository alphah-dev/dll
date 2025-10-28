#include "Network.h"
#include <iostream>
#include <cstring>

NetworkSocket::NetworkSocket() : sock(INVALID_SOCKET), initialized(false) {
    InitializeWinsock();
}

NetworkSocket::~NetworkSocket() {
    CloseSocket();
    CleanupWinsock();
}

void NetworkSocket::InitializeWinsock() {
#ifdef _WIN32
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        throw NetworkException("WSAStartup failed with error: " + std::to_string(result));
    }
    initialized = true;
#endif
}

void NetworkSocket::CleanupWinsock() {
#ifdef _WIN32
    if (initialized) {
        WSACleanup();
        initialized = false;
    }
#endif
}

bool NetworkSocket::CreateSocket() {
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
#ifdef _WIN32
        std::cerr << "Socket creation failed with error: " << WSAGetLastError() << std::endl;
#else
        std::cerr << "Socket creation failed" << std::endl;
#endif
        return false;
    }
    return true;
}

bool NetworkSocket::BindSocket(int port) {
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);
    
    if (bind(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
#ifdef _WIN32
        std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
#else
        std::cerr << "Bind failed" << std::endl;
#endif
        CloseSocket();
        return false;
    }
    return true;
}

bool NetworkSocket::ListenSocket(int backlog) {
    if (listen(sock, backlog) == SOCKET_ERROR) {
#ifdef _WIN32
        std::cerr << "Listen failed with error: " << WSAGetLastError() << std::endl;
#else
        std::cerr << "Listen failed" << std::endl;
#endif
        CloseSocket();
        return false;
    }
    return true;
}

SOCKET NetworkSocket::AcceptConnection() {
    SOCKET clientSocket = accept(sock, NULL, NULL);
    if (clientSocket == INVALID_SOCKET) {
#ifdef _WIN32
        std::cerr << "Accept failed with error: " << WSAGetLastError() << std::endl;
#else
        std::cerr << "Accept failed" << std::endl;
#endif
        return INVALID_SOCKET;
    }
    return clientSocket;
}

bool NetworkSocket::ConnectToServer(const std::string& ipAddress, int port) {
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    
#ifdef _WIN32
    inet_pton(AF_INET, ipAddress.c_str(), &serverAddr.sin_addr);
#else
    inet_pton(AF_INET, ipAddress.c_str(), &serverAddr.sin_addr);
#endif
    
    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
#ifdef _WIN32
        std::cerr << "Connect failed with error: " << WSAGetLastError() << std::endl;
#else
        std::cerr << "Connect failed" << std::endl;
#endif
        CloseSocket();
        return false;
    }
    return true;
}

int NetworkSocket::SendData(const std::string& data) {
    int result = send(sock, data.c_str(), static_cast<int>(data.length()), 0);
    if (result == SOCKET_ERROR) {
#ifdef _WIN32
        std::cerr << "Send failed with error: " << WSAGetLastError() << std::endl;
#else
        std::cerr << "Send failed" << std::endl;
#endif
        return -1;
    }
    return result;
}

int NetworkSocket::SendData(SOCKET clientSocket, const std::string& data) {
    int result = send(clientSocket, data.c_str(), static_cast<int>(data.length()), 0);
    if (result == SOCKET_ERROR) {
#ifdef _WIN32
        std::cerr << "Send failed with error: " << WSAGetLastError() << std::endl;
#else
        std::cerr << "Send failed" << std::endl;
#endif
        return -1;
    }
    return result;
}

std::string NetworkSocket::ReceiveData(int bufferSize) {
    char* buffer = new char[bufferSize];
    memset(buffer, 0, bufferSize);
    
    int result = recv(sock, buffer, bufferSize - 1, 0);
    std::string data;
    
    if (result > 0) {
        data = std::string(buffer, result);
    } else if (result == 0) {
        std::cout << "Connection closed" << std::endl;
    } else {
#ifdef _WIN32
        std::cerr << "Recv failed with error: " << WSAGetLastError() << std::endl;
#else
        std::cerr << "Recv failed" << std::endl;
#endif
    }
    
    delete[] buffer;
    return data;
}

std::string NetworkSocket::ReceiveData(SOCKET clientSocket, int bufferSize) {
    char* buffer = new char[bufferSize];
    memset(buffer, 0, bufferSize);
    
    int result = recv(clientSocket, buffer, bufferSize - 1, 0);
    std::string data;
    
    if (result > 0) {
        data = std::string(buffer, result);
    } else if (result == 0) {
        std::cout << "Connection closed" << std::endl;
    } else {
#ifdef _WIN32
        std::cerr << "Recv failed with error: " << WSAGetLastError() << std::endl;
#else
        std::cerr << "Recv failed" << std::endl;
#endif
    }
    
    delete[] buffer;
    return data;
}

void NetworkSocket::CloseSocket() {
    if (sock != INVALID_SOCKET) {
#ifdef _WIN32
        closesocket(sock);
#else
        close(sock);
#endif
        sock = INVALID_SOCKET;
    }
}

void NetworkSocket::CloseSocket(SOCKET clientSocket) {
    if (clientSocket != INVALID_SOCKET) {
#ifdef _WIN32
        closesocket(clientSocket);
#else
        close(clientSocket);
#endif
    }
}
