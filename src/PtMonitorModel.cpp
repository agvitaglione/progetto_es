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
DataStore* PtMonitorModel::dataStore = nullptr;
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

    if(dataStore != nullptr) {
        delete dataStore;
    }
}

bool PtMonitorModel::getData(MessageType& message)  {
    return queue.pop(message);
}

bool PtMonitorModel::setDataStore(std::string usbLabel) {

    if(dataStore != nullptr) {
        delete dataStore;
    }

    dataStore = new DataStore(usbLabel);

    if(!dataStore->isOpen()) {
        delete dataStore;
        dataStore = nullptr;
        return false;
    }
    else {
        return true;
    }
}

bool PtMonitorModel::isDataStoreSet() const {
    if(dataStore != nullptr && dataStore->isOpen()) {
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
        if(dataStore != nullptr && dataStore->isOpen()) {
            log = "[" + std::to_string(t) + "] id-sensor: " + std::to_string(id) + " temperature: " + std::to_string(temperature) + " pressure: " + std::to_string(pressure);
            dataStore->write(log);
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

bool mountUSB(USB_t usb) {
    std::string username = exec("echo $USERNAME");
    std::string destPath = "/home/" + username + "/" + usb.label + "/ptmonitorLog.txt";
    std::string cmd = "mount " + usb.path + " " + destPath;
    std::string result = exec(cmd.c_str());

    if(result == "") {
        return true;
    }
    else {
        return false;
    }
}

void PtMonitorModel::deleteDataStore() {
    if(dataStore != nullptr) {
        delete dataStore;
    }
}