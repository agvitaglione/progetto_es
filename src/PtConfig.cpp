#include "PtConfig.h"



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
    
    for(int i = 0; i < 2; i++) {
        std::getline(file, line, '=');
        
        if(line == "NUMBER_OF_AXIS") {
            std::getline(file, line);
            numberOfAxis = std::stoi(line);
        }
        else if (line == "NUMBER_OF_TYRE_PER_AXIS") {
            std::getline(file, line);
            numberOfTyrePerAxis = std::stoi(line);
        }

    }

    // -------------------------

    // ------------------------- Get sensors ID

    TyrePosition position;

    for(int axis = 0; axis < numberOfAxis; axis++) {
        for(int tyre = 0; tyre < numberOfTyrePerAxis; tyre++) {

            std::getline(file, line);
            position.axis = axis;
            position.tyre = tyre;
            positions.insert(std::pair<std::string, TyrePosition>(line, position));

        }
    }

    // -------------------------


}

void PtConfig::saveNewConfiguration(const int numberOfAxis, const int numberOfTyrePerAxis, std::string *id) {
    
    file.seekg(std::fstream::beg);

    file << "NUMBER_OF_AXIS=" << numberOfAxis << std::endl;
    file << "NUMBER_OF_TYRE_PER_AXIS=" << numberOfTyrePerAxis << std::endl;

    for(int i = 0; i < numberOfAxis * numberOfTyrePerAxis; i++) {
            file << id[i] << std::endl;
    }
}

int PtConfig::getNumberOfAxis() const {
    return numberOfAxis;
}

int PtConfig::getNumberOfTyrePerAxis() const {
    return numberOfTyrePerAxis;
}

int PtConfig::getAxisFromId(const std::string id) {
    return positions[id].axis;
}

int PtConfig::getTyreFromId(const std::string id) {
    return positions[id].tyre;
}