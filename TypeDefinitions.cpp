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

const MessageType& MessageType::operator=(const MessageType& other) {

    time = other.time;
    
    for(int i = 0; i < 2; i++) {
        fl[i] = other.fl[i];
        fr[i] = other.fr[i];
        rl[i] = other.rl[i];
        rr[i] = other.rr[i];
    }

    return *this;
}

MessageType::MessageType(const MessageType& other) {
    time = other.time;
    
    for(int i = 0; i < 2; i++) {
        fl[i] = other.fl[i];
        fr[i] = other.fr[i];
        rl[i] = other.rl[i];
        rr[i] = other.rr[i];
    }
}