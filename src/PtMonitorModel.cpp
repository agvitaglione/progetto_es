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
DataStore PtMonitorModel::dataStore("");
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

void PtMonitorModel::setDataStore(std::string path) {
    dataStore.setPath(path);
}

bool PtMonitorModel::isDataStoreSet() const {
    if(dataStore.isOpen()) {
        return true;
    }
    else {
        return false;
    }
}

void PtMonitorModel::readDataFromModule() {

    std::string log;

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

    //FOR TEST
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
        
        //std::this_thread::sleep_for(std::chrono::seconds(1));
        
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
        //uint32_t id = ids[count];
        //count = (count + 1) % 11;
        uint32_t temperature (frame.data[1]); // °C
        //uint32_t temperature = temperatures[count_temp] + 52;
        //count_temp = (count_temp + 1) % 6;
        temperature = temperature - 52;
        uint32_t pressure (frame.data[2] + frame.data[3] & 0x1); // mBar
        //uint32_t pressure = temperatures[count_temp];
        pressure = pressure * 40;
        int t = (int) time(NULL);
        queue.push(MessageType(id, temperature, pressure, t));

        // WRITE LOG FILE
        if(dataStore.isOpen()) {
            log = "[" + std::to_string(t) + "] id-sensor: " + std::to_string(id) + " temperature: " + std::to_string(temperature) + " pressure: " + std::to_string(pressure);
            dataStore.write(log);
        }
    }
}

std::vector<USB_t> PtMonitorModel::getUSBList() const {

	std::vector<USB_t> usbList;
	std::vector<std::string> strList;
	std::string result = exec("blkid -t TYPE=vfat");
	result += exec("blkid -t TYPE=ext4");
	
	size_t pos = 0;
	std::string token;
	while ((pos = result.find("\n")) != std::string::npos) {
		strList.push_back(result.substr(0, pos));
		result.erase(0, pos + 1);
	}	

	std::string toFind = ": LABEL=";
	size_t firstIndex, lastIndex;
	std::string path;
	USB_t usb;
	for(auto elem : strList) {
		firstIndex = elem.find(toFind);

		if(firstIndex != std::string::npos) {


			firstIndex +=  toFind.length() + 1;
			lastIndex = elem.find('\"', firstIndex);
			usb.label = elem.substr(firstIndex, lastIndex - firstIndex);
			usb.path = elem.substr(0, elem.find(":"));
			usbList.push_back(usb);
		}
	}

    
    return usbList;
}

bool PtMonitorModel::mountUSB(USB_t usb, std::string path) {
    std::string cmd = "mkdir " + path;
    exec(cmd.c_str());

    /**
     * @brief TODO SUDO ESCALETION MUST BE REMOVED FROM CMD
     * Set account permissions properly in order to permit mount, umount, mkdir
     */
    cmd = "echo admin1234 | sudo -S umount -l " + usb.path;
    exec(cmd.c_str());

    cmd = "echo admin1234 | sudo -S mount -o rw,umask=0 " + usb.path + " " + path;
    std::string result = exec(cmd.c_str());


    // TODO Not always return true
    return true;
}

void PtMonitorModel::deleteDataStore() {
    dataStore.setPath("");
}