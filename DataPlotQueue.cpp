#include "DataPlotQueue.h"

DataPlotQueue::DataPlotQueue(const int size) {
    this->size = size;
    queue = new MessageType[size];
    nelem = 0;
    head = 0;
    tail = 0;
}


DataPlotQueue::DataPlotQueue(const DataPlotQueue& other)  {
    this->size = other.size;
    this->queue = new MessageType[other.size];
    this->nelem = other.nelem;
    this->head = other.head;
    this->tail = other.tail;

    for(int i = 0; i < size; i++) {
        this->queue[i] = other.queue[i];
    }
}


DataPlotQueue::~DataPlotQueue()  {
    delete[] queue;
}

void DataPlotQueue::push(const MessageType& message) {
  
    queue[tail] = message;
    tail = (tail + 1) % size;

    if(nelem < size) {
       nelem++;
    }
    else {
        head = (head + 1) % size;
    }
}


void DataPlotQueue::getData(MessageType *messageArray) const {

    int i = head;
    for(int k = 0; k < nelem; k++) {
        messageArray[k] = queue[i];
        i = (i + 1) % size;
    }
}
