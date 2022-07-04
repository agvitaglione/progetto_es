#ifndef __DATA_PLOT_QUEUE_H__
#define __DATA_PLOT_QUEUE_H__
#include "TypeDefinitions.h"

class DataPlotQueue {

    public:
        DataPlotQueue(const int size);
        DataPlotQueue(const DataPlotQueue& other);
        ~DataPlotQueue();

        // IF THE QUEUE IS FULL, REMOVE THE OLDEST ELEMENT
        void push(const MessageType& message);

        // RETURN X Y ARRAY IN CHRONOLOGICAL ORDER
        void getData(MessageType *messageArray) const;

        int getNelem() const {return nelem;}
        int getSize() const {return size;}

    private:

        // VECTOR OF SAMPLES TO BE PLOT
	    MessageType *queue;
        int head;
        int tail;
        int nelem;
        int size;
};

#endif // __DATA_PLOT_QUEUE_H__