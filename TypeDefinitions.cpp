#include "TypeDefinitions.h"
std::string toString(TyreType tyre) {

    switch(tyre) {
        case TyreType::FL : return "FL";
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