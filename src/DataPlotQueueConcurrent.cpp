#include "DataPlotQueueConcurrent.h"

DataPlotQueueConcurret::DataPlotQueueConcurret(const int size) : DataPlotQueue(size) {
}

DataPlotQueueConcurret::DataPlotQueueConcurret(const DataPlotQueueConcurret& other) : DataPlotQueue(other) {
}


void DataPlotQueueConcurret::push(const MessageType& message) {
   mutex.lock();
   DataPlotQueue::push(message);
   mutex.unlock();
}

bool DataPlotQueueConcurret::pop(MessageType& message) {
    bool result;
    mutex.lock();
    result = DataPlotQueue::pop(message);
    mutex.unlock();
    return result;
}
