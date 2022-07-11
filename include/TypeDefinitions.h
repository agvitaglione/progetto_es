#ifndef __TYPEDEFINITIONS_H__
#define __TYPEDEFINITIONS_H__
#include <string>
#include <memory>

/// Default queue size
#define MAX_QUEUE_SIZE 11

/// Type of plottable graph (temperature graph or pressure grap
typedef enum {TEMPERATURE, PRESSURE} MeasureType; 


/**
 * @brief DataTypre rappresents the data to plot.
 *      - x: vector of x value
 *      - y: vector of y value
 * x and y must have the same length.
 * 
 */
typedef struct {
    double x[MAX_QUEUE_SIZE];
    double y[MAX_QUEUE_SIZE];
} DataType;

/**
 * @brief USB_t rappresents an USB.
 *      - path: /dev/{USB} path
 *      - label: USB's label
 */
typedef struct {
	std::string path;
	std::string label;
} USB_t;

/**
 * @brief This class rappresents a messagge sended by a sensor.
 * 
 * Variables:
 *      - id: The id of the sensor.
 *      - temperature: The measured temperature.
 *      - pressure: The measured pressure.
 *      - time: The timestamop when the message has been received. 
 * 
 */
class MessageType {

    public:

        uint32_t id;
        uint32_t temperature;
        uint32_t pressure;
        int time;

        /// Constructors
        MessageType();
        MessageType(uint32_t id, uint32_t temperature, uint32_t pressure, int time);
        MessageType(const MessageType& other);
};

/**
 * @brief Returns the unit mesure of a MesureType.
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