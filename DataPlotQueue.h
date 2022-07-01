#ifndef __DATA_PLOT_QUEUE_H__
#define __DATA_PLOT_QUEUE_H__
#include <iostream>


class DataPlotQueue {

    public:
        DataPlotQueue(const int size);
        DataPlotQueue(const DataPlotQueue& other);
        ~DataPlotQueue();

        // IF THE QUEUE IS FULL, REMOVE THE OLDEST ELEMENT
        void push(const double xVal, const double yVal);

        // RETURN X Y ARRAY IN CHRONOLOGICAL ORDER
        void getData(double *xData, double *yData) const;

        int getNelem() const {return nelem;}

    private:

        // VECTOR OF SAMPLES TO BE PLOT
	    double *x;
	    double *y;
        int head;
        int tail;
        int nelem;
        int size;
};

#endif // __DATA_PLOT_QUEUE_H__