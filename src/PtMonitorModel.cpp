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

DataPlotQueueConcurret PtMonitorModel::queue(60);
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

    uint32_t ids[] = {0x28E311B,
    0x28E241E,
    0x28E2F1E,
    0x28B1826,
    0x2900E23,
    0x28E0D14,
    0x28B3827,
    0x2912717,
    0x28B1426,
    0x28D1832,
    0x000003A};
    uint32_t temperatures[] = {5, 10, 15, 20, 25, 30};
    int count = 0;
    int count_temp = 0;
    
	while(stopThread == 0) {
        
    	nbytes = read(s, &frame, sizeof(struct can_frame));

		if (nbytes < 0) {
		        // perror("can raw socket read");
		        continue;
		}
        
		// paranoid check ... 
		if (nbytes < sizeof(struct can_frame)) {
		        // fprintf(stderr, "read: incomplete CAN frame\n");
		        continue;
		}
		
		/* do something with the received CAN frame */
		uint32_t id (frame.can_id & (uint32_t)0x1FFFFFFF);
        // uint32_t id = ids[count];
        // count = (count + 1) % 11;
        uint32_t temperature (frame.data[1]); // °C
        // uint32_t temperature = temperatures[count_temp] + 52;
        // count_temp = (count_temp + 1) % 6;
        temperature = temperature - 52;
        uint32_t pressure (frame.data[2] + frame.data[3] & 0x1); // mBar
        //uint32_t pressure = temperatures[count_temp];
        pressure = pressure * 40;
        int t = (int) time(NULL);
        queue.push(MessageType(id, temperature, pressure, t));

        /*
        std::cout << "Messaggio" << std::endl;
        std::cout << "id: " << id << std::endl;
        std::cout << "temperature: " << temperature << std::endl;
        std::cout << "pressure: " << pressure << std::endl << std::endl;
        */
    }
}