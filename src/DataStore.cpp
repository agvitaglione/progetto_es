#include "DataStore.h"
#include "TypeDefinitions.h"

DataStore::DataStore(std::string usbLabel){
    std::string username = exec("echo $USERNAME");
    std::string path = "/home/" + username + "/" + usbLabel + "/ptmonitorLog.txt";

    try {
        file.open(path, std::fstream::out | std::fstream::app);
    } catch(...) {
        // NOTHING TO DO. THE FILE IS NOT OPENED
    }
}

DataStore::~DataStore() {
    if(file.is_open()) {
        file.close();
    }
}

void DataStore::setUsbLabel(std::string usbLabel) {

    std::string username = exec("echo $USERNAME");
    std::string path = "/home/" + username + "/" + usbLabel + "/ptmonitorLog.txt";

    mutex.lock();
    if(file.is_open()) {
        file.close();
    }
    file.open(path, std::fstream::out | std::fstream::app);
    mutex.unlock();
}

void DataStore::write(std::string line) {
    mutex.lock();
    if(file.is_open()) {
        file << line << std::endl;
    }
    mutex.unlock();
}

bool DataStore::isOpen() const {
    return file.is_open();
}