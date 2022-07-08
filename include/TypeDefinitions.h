#ifndef __TYPEDEFINITIONS_H__
#define __TYPEDEFINITIONS_H__
#include <string>
#include <iostream>
#define MAX_QUEUE_SIZE 11

// TYPE OF PLOTTABLE GRAPH (TEMPERATURE GRAPH OR PRESSURE GRAPH)
typedef enum {TEMPERATURE, PRESSURE} MeasureType; 

// STRUCT THAT CONTAINS THE X Y VALUE TO PLOT, ACCORDING TO THE TYRE
typedef struct {
    double x[MAX_QUEUE_SIZE];
    double y[MAX_QUEUE_SIZE];
} DataType;

class MessageType {

    public:
        uint32_t id;
        uint32_t temperature;
        uint32_t pressure;
        int time;

        MessageType();
        MessageType(uint32_t id, uint32_t temperature, uint32_t pressure, int time);
        MessageType(const MessageType& other);
};

std::string toUnit(MeasureType measure);
#endif // __TYPEDEFINITIONS_H__