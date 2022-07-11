#include "PtConfig.h"
#include <sstream>
#include <iostream>


PtConfig* PtConfig::getInstance() {
    static PtConfig ptConfig;
    return &ptConfig;
}

PtConfig::~PtConfig() {
    if(file.is_open()) {
        file.close();
    }
}

void PtConfig::readFile(const std::string fileName) {

    if(file.is_open()) {
        return;
    }

    file.open(fileName, std::fstream::in | std::fstream::out );

    // ------------------------- Get configuration parameters from file

    std::string line;

    // TODO: check exception for error in configuration file
    
    for(int i = 0; i < 4; i++) {
        std::getline(file, line, '=');
        
        if(line == "NUMBER_OF_AXIS") {
            std::getline(file, line);
            numberOfAxis = std::stoi(line);
        }
        else if (line == "NUMBER_OF_TYRES_PER_AXIS") {
            std::getline(file, line);
            numberOfTirePerAxis = std::stoi(line);
        }
        else if(line == "RECEIVER_IP") {
            std::getline(file, line);
            recIP = line;
        }
        else if(line == "RECEIVER_PORT") {
            std::getline(file, line);
            recPort = stoi(line);
        }

    }

    // -------------------------

    // ------------------------- Get sensors ID

    TirePosition position;

    for(int axis = 0; axis < numberOfAxis; axis++) {
        for(int tire = 0; tire < numberOfTirePerAxis; tire++) {

            std::getline(file, line);
            position.axis = axis;
            position.tire = tire;

            if(line != "NONE") {
                uint32_t x;   
                std::stringstream ss;
                ss << std::hex << line ;
                ss >> x;
                positions.insert(std::pair<uint32_t, TirePosition>(x, position));
            }
        }
    }

    // -------------------------


}

void PtConfig::saveNewConfiguration(const int numberOfAxis, const int numberOfTirePerAxis, std::string *id) {
    
    file.seekg(std::fstream::beg);

    file << "NUMBER_OF_AXIS=" << numberOfAxis << std::endl;
    file << "NUMBER_OF_TYRES_PER_AXIS=" << numberOfTirePerAxis << std::endl;

    for(int i = 0; i < numberOfAxis * numberOfTirePerAxis; i++) {
            file << id[i] << std::endl;
    }
}

int PtConfig::getNumberOfAxis() const {
    return numberOfAxis;
}

int PtConfig::getNumberOfTirePerAxis() const {
    return numberOfTirePerAxis;
}

int PtConfig::getAxisFromId(const uint32_t id) {
    return positions[id].axis;
}

int PtConfig::getTireFromId(const uint32_t id) {
    return positions[id].tire;
}

bool PtConfig::isValidId(const uint32_t id) const {
    if(positions.count(id) > 0) return true;
    else return false;
}


std::string PtConfig::getRecIP() const {
    return recIP;
}


uint16_t PtConfig::getRecPort() const {
    return recPort;
}