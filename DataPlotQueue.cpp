#include "DataPlotQueue.h"

DataPlotQueue::DataPlotQueue(const int size) {
    this->size = size;
    x = new float[size];
    y = new float[size];
    nelem = 0;
    head = 0;
    tail = 0;
}


DataPlotQueue::DataPlotQueue(const DataPlotQueue& other)  {
    this->size = other.size;
    this->x = new float[other.size];
    this->y = new float[other.size];
    this->nelem = other.nelem;
    this->head = other.head;
    this->tail = other.tail;

    for(int i = 0; i < size; i++) {
        this->x[i] = other.x[i];
        this->y[i] = other.y[i];
    }
}


DataPlotQueue::~DataPlotQueue()  {
    delete[] x;
    delete[] y;
}

void DataPlotQueue::push(const float xVal, const float yVal) {
  
    x[tail] = xVal;
    y[tail] = yVal;
    tail = (tail + 1) % size;

    std::cout << tail << "\t" << std::endl;

    if(nelem < size) {
       nelem++;
    }
    else {
        head = (head + 1) % size;
    }
}


void DataPlotQueue::getData(float *xData, float *yData) const {

    int i = head;
    for(int k = 0; k < nelem; k++) {
        xData[k] = x[i];
        yData[k] = y[i];
        i = (i + 1) % size;
    }
}
