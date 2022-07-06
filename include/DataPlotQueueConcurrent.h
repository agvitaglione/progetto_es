#ifndef __DATA_PLOT_QUEUE_CONCURRET_H__
#define __DATA_PLOT_QUEUE_CONCURRET_H__
#include "DataPlotQueue.h"
#include "TypeDefinitions.h"
#include <mutex>

class DataPlotQueueConcurret : public DataPlotQueue {

    public:
        DataPlotQueueConcurret(const int size);
        DataPlotQueueConcurret(const DataPlotQueueConcurret& other);

        // IF THE QUEUE IS FULL, REPLACE THE OLDEST ELEMENT
        virtual void push(const MessageType& message);
        virtual bool pop(MessageType& message);

    private:
        std::mutex mutex;
};

#endif //__DATA_PLOT_QUEUE_CONCURRET_H__