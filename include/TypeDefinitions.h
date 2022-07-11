#ifndef __TYPEDEFINITIONS_H__
#define __TYPEDEFINITIONS_H__
#include <string>
#include <memory>

/// Default queue size
#define MAX_QUEUE_SIZE 11

// TYPE OF PLOTTABLE GRAPH (TEMPERATURE GRAPH OR PRESSURE GRAPH)
typedef enum {TEMPERATURE, PRESSURE} MeasureType; 

// STRUCT THAT CONTAINS THE X Y VALUE TO PLOT, ACCORDING TO THE TYRE
typedef struct {
    double x[MAX_QUEUE_SIZE];
    double y[MAX_QUEUE_SIZE];
} DataType;

typedef struct {
	std::string path;
	std::string label;
} USB_t;

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

/**
 * @brief Returns the unit mesure of a Mesur type.
 * 
 * 
 * @param measure 
 * @return std::string "°C" if TEMPERATURE, " bar" if PRESSURE.
 */
std::string toUnit(MeasureType measure);

/**
 * @brief Exec terminal commands
 * 
 * @return string output
 */
std::string exec(const char* cmd);

#endif // __TYPEDEFINITIONS_H__