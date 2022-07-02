#ifndef __TYPEDEFINITIONS_H__
#define __TYPEDEFINITIONS_H__
#include <string>

// TYPE OF PLOTTABLE GRAPH (TEMPERATURE GRAPH OR PRESSURE GRAPH)
typedef enum {TEMPERATURE, PRESSURE} MeasureType; 

// TYRE TYPE
typedef enum {FL, FR, RL, RR} TyreType;

// STRUCT THAT CONTAINS THE X Y VALUE TO PLOT, ACCORDING TO THE TYRE
typedef struct {
    double *x;
    double *y;
    TyreType tyre;
} DataType;

typedef struct {
    float fl[2];
    float fr[2];
    float rl[2];
    float rr[2];
} MessageType;


std::string toString(TyreType tyre);
std::string toUnit(MeasureType measure);
#endif // __TYPEDEFINITIONS_H__