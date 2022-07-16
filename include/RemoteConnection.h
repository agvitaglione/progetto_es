#ifndef __REMOTE_CONNECTION_H__
#define __REMOTE_CONNECTION_H__

#include <string>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>


class RemoteConnection {

    public:

        /**
         * @brief Construct a new Remote Connection object
         * 
         */
        RemoteConnection();

        /**
         * @brief Destroy the Remote Connection object
         * 
         */
        ~RemoteConnection();

        /**
         * @brief Set the connection to a remote host. 
         * 
         * @param IP 
         * @param port 
         */
        void setConnection(std::string IP, uint16_t port);

        /**
         * @brief Send a message to a remote host. Should be called after setConnection.
         * 
         * @param message 
         */
        void write(std::string message);

        /**
         * @brief It is usually called in case you are not sure whether connetion has been set up beforehand.
         * 
         * @return true 
         * @return false 
         */
        bool isConnected() const;

    private:

        int sock; 
        struct sockaddr_in serv_addr;
};

#endif // __REMOTE_CONNECTION_H__