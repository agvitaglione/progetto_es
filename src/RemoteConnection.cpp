#include "RemoteConnection.h"
#include "TypeDefinitions.h"



RemoteConnection::RemoteConnection() {
    sock = 0;
}

RemoteConnection::~RemoteConnection() {
    if(sock != 0) {
        close(sock);
    }
}

void RemoteConnection::setConnection(std::string IP, uint16_t port) {

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_pton(AF_INET, IP.c_str(), &serv_addr.sin_addr);
    bind(sock, (sockaddr*)&serv_addr, sizeof(serv_addr));
}

void RemoteConnection::write(std::string message) {
    if(sock != 0) {
        sendto(sock, message.c_str(), message.length(), 0, (sockaddr*)&serv_addr, sizeof(serv_addr));
    }
}

bool RemoteConnection::isConnected() const {
    if(sock != 0) {
        return true;
    }
    else {
        return false;
    }
}
