#include "TypeDefinitions.h"

std::string toString(TyreType tyre) {

    switch(tyre) {
        case TyreType::FL: return "FL";
        case TyreType::FR: return "FR";
        case TyreType::RL: return "RL";
        case TyreType::RR: return "RR";
        default: return NULL;
    }
} 

std::string toUnit(MeasureType measure) {
    switch(measure) {
        case MeasureType::TEMPERATURE: return "°C";
        case MeasureType::PRESSURE: return " bar";
        default: return NULL;
    }
}

MessageType::MessageType() {
    this->id = 0;
    this->temperature = 0;
    this->pressure = 0;
    this->time = 0;
}

MessageType::MessageType(uint32_t id, uint32_t temperature, uint32_t pressure, int time) {
    this->id = id;
    this->temperature = temperature;
    this->pressure = pressure;
    this->time = time;
}