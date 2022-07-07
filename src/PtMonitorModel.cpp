#include "PtMonitorModel.h"
#include <signal.h>
#include <time.h>
#include <iomanip>

// FOR CAN MESSAGE
#include <sys/socket.h>
#include <sys/types.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include "/usr/include/linux/can.h"

DataPlotQueueConcurret PtMonitorModel::queue(MAX_QUEUE_SIZE);
static int stopThread = 0;

PtMonitorModel* PtMonitorModel::getInstance() {
    static PtMonitorModel model;
    return &model;
}

PtMonitorModel::PtMonitorModel() {
    readDataFromModuleThread = new std::thread(readDataFromModule);
}

PtMonitorModel::~PtMonitorModel() {
    stopThread = 1;
    readDataFromModuleThread->join();
    delete readDataFromModuleThread;
}

bool PtMonitorModel::getData(MessageType& message)  {
    return queue.pop(message);
}

void PtMonitorModel::readDataFromModule() {

    int s;
    struct sockaddr_can addr;
    struct ifreq ifr;

    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    strcpy(ifr.ifr_name, "can0" );
    ioctl(s, SIOCGIFINDEX, &ifr);

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    bind(s, (struct sockaddr *)&addr, sizeof(addr));
    
    
    struct can_frame frame;
    int nbytes;
    
	while(stopThread == 0) {
		
    	nbytes = read(s, &frame, sizeof(struct can_frame));

		if (nbytes < 0) {
		        // perror("can raw socket read");
		        continue;
		}
        
		/* paranoid check ... */
		if (nbytes < sizeof(struct can_frame)) {
		        // fprintf(stderr, "read: incomplete CAN frame\n");
		        continue;
		}

		/* do something with the received CAN frame */
		uint32_t id (frame.can_id & (uint32_t)0x1FFFFFFF);
        uint32_t temperature (frame.data[1]); // °C
        temperature = temperature - 52;
        uint32_t pressure (frame.data[2] + frame.data[3] & 0x1); // mBar
        pressure = pressure * 40;
        int t = (int) time(NULL);
        queue.push(MessageType(id, temperature, pressure, t));
    }
}