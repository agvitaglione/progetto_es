#include "DataStore.h"

DataStore::DataStore(std::string path) : file(path, std::fstream::out | std::fstream::app) {
}

DataStore::~DataStore() {
    if(file.is_open()) {
        file.close();
    }
}

void DataStore::setPath(std::string path) {
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