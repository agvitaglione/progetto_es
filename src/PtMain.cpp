
#include <thread>
#include <iostream>
#include "PtMonitorView.h"
#include "PtMonitorControl.h"
#include "PtMonitorModel.h"
#include "DataPlotQueue.h"
#include "TypeDefinitions.h"
#include "PtConfig.h"
#include "RemoteConnection.h"

// Server socket
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>


RemoteConnection rc;

void task() {
    int count = 0;
    std::cout << "[CLIENT] Inizio invio messaggi" << std::endl;
    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        rc.write("Messaggio " + std::to_string(count++) + '\0');
    }
}

int main() {

    /*
    PtConfig *ptconfig = PtConfig::getInstance();
    ptconfig->readFile("../config.txt");

    PtMonitorView *view = PtMonitorView::getInstance();
    PtMonitorModel *model = PtMonitorModel::getInstance();
    PtMonitorControl *control = PtMonitorControl::getInstance(view, model);
    */

    // SET LOG
    /*
    if(ptconfig->getLogPath() != "") {
        model->setDataStore(ptconfig->getLogPath());
    }*/

    // view->startRoutine();


    
    rc.setConnection("0.0.0.0", 8080);

    std::thread thread(task);

    // set server
    int server_fd, new_socket;
    struct sockaddr_in address;
    server_fd = socket(AF_INET, SOCK_DGRAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);
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