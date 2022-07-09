
#include <thread>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>



int main() {

    // set server
    int server_fd, new_socket;
    struct sockaddr_in address;
    server_fd = socket(AF_INET, SOCK_DGRAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(7000);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);

    std::cout << "[SERVER] Attesa di messaggi" << std::endl;
    
    while(true) {
        struct sockaddr sock;
        char buffer[1024];
        recvfrom(server_fd, buffer, 1024, 0, &sock, (socklen_t*)sizeof(sock));
        printf("%s\n", buffer);
    }

    return 0;
}