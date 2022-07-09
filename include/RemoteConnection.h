#ifndef __REMOTE_CONNECTION_H__
#define __REMOTE_CONNECTION_H__

#include "DataPlotQueueConcurrent.h"
#include <string>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>


class RemoteConnection {

    public:

        RemoteConnection();
        ~RemoteConnection();

        void setConnection(std::string IP, uint16_t port);
        void write(std::string message);

    private:

        int sock; 
        struct sockaddr_in serv_addr;
};

#endif // __REMOTE_CONNECTION_H__