#ifndef __TYPEDEFINITIONS_H__
#define __TYPEDEFINITIONS_H__
#include <string>
#include <iostream>

// TYPE OF PLOTTABLE GRAPH (TEMPERATURE GRAPH OR PRESSURE GRAPH)
typedef enum {TEMPERATURE, PRESSURE} MeasureType; 

// TYRE TYPE
typedef enum {FL, FR, RL, RR} TyreType;

// STRUCT THAT CONTAINS THE X Y VALUE TO PLOT, ACCORDING TO THE TYRE
typedef struct {
    double *x;
    double *y;
} DataType;

class MessageType {
    uint32_t id;
    uint32_t temperature;
    uint32_t pressure;
    int time;

    MessageType(uint32_t id, uint32_t temperature, uint32_t pressure, int time);
} ;


std::string toString(TyreType tyre);
std::string toUnit(MeasureType measure);
#endif // __TYPEDEFINITIONS_H__