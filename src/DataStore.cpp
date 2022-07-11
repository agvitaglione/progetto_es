#include "DataStore.h"
#include "TypeDefinitions.h"
#include <unistd.h>

DataStore::DataStore(std::string path){
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

void DataStore::closeFile() {

    try {
        if(file.is_open()) {
            file.close();
        }
    } catch(...) {
        // NOTHING TO DO
    }
}

void DataStore::setPath(std::string path) {

    mutex.lock();
    if(file.is_open()) {
        file.close();
    }
    if(path != "") {
        file.open(path, std::fstream::out | std::fstream::app);
    }
    mutex.unlock();
}

void DataStore::write(std::string line) {
    mutex.lock();
    try{
        if(file.is_open()) {
            file << line << std::endl;
        }
    }
    catch(...) {
        // NOTHING TO DO: ERROR IN WRITING LOG FILE
    }
    mutex.unlock();
}

bool DataStore::isOpen() const {

    try {
        return file.is_open();
    }
    catch(...) {
        return false;
    }
}