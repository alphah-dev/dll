#ifndef NETWORK_H
#define NETWORK_H

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    #ifdef NETWORK_EXPORTS
        #define NETWORK_API __declspec(dllexport)
    #else
        #define NETWORK_API __declspec(dllimport)
    #endif
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <unistd.h>
    #define NETWORK_API
    #define SOCKET int
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
#endif

#include <string>
#include <stdexcept>

class NETWORK_API NetworkSocket {
private:
    SOCKET sock;
    bool initialized;

public:
    NetworkSocket();
    ~NetworkSocket();
    
    void InitializeWinsock();
    void CleanupWinsock();
    
    bool CreateSocket();
    bool BindSocket(int port);
    bool ListenSocket(int backlog = 5);
    SOCKET AcceptConnection();
    bool ConnectToServer(const std::string& ipAddress, int port);
    
    int SendData(const std::string& data);
    int SendData(SOCKET clientSocket, const std::string& data);
    std::string ReceiveData(int bufferSize = 1024);
    std::string ReceiveData(SOCKET clientSocket, int bufferSize = 1024);
    
    void CloseSocket();
    void CloseSocket(SOCKET clientSocket);
    
    SOCKET GetSocket() const { return sock; }
    bool IsValid() const { return sock != INVALID_SOCKET; }
};

class NETWORK_API NetworkException : public std::runtime_error {
public:
    NetworkException(const std::string& message) : std::runtime_error(message) {}
};

#endif
