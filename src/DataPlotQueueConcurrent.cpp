#include "DataPlotQueueConcurrent.h"

DataPlotQueueConcurret::DataPlotQueueConcurret(const int size) : DataPlotQueue(size) {
}

DataPlotQueueConcurret::DataPlotQueueConcurret(const DataPlotQueueConcurret& other) : DataPlotQueue(other) {
}


void DataPlotQueueConcurret::push(const MessageType& message) {
   mutex.lock();
   std::cout << "Mutex preso" << std::endl;
   DataPlotQueue::push(message);
   mutex.unlock();
   std::cout << "Mutex rilasciato" << std::endl;
}

bool DataPlotQueueConcurret::pop(MessageType& message) {
    bool result;
    mutex.lock();
    std::cout << "Mutex preso" << std::endl;
    result = DataPlotQueue::pop(message);
    mutex.unlock();
    std::cout << "Mutex rilasciato" << std::endl;
    return result;
}
