#ifndef __DATA_PLOT_QUEUE_H__
#define __DATA_PLOT_QUEUE_H__

class DataPlotQueue {

    public:
        DataPlotQueue(const int size);
        DataPlotQueue(const DataPlotQueue& other);
        ~DataPlotQueue();

        // IF THE QUEUE IS FULL, REMOVE THE OLDEST ELEMENT
        void push(const float xVal, const float yVal);

        // RETURN X Y ARRAY IN CHRONOLOGICAL ORDER
        void getData(float *xData, float *yData) const;

        int getNelem() const {return nelem;}

    private:

        // VECTOR OF SAMPLES TO BE PLOT
	    float *x;
	    float *y;
        int head;
        int tail;
        int nelem;
        int size;
};

#endif // __DATA_PLOT_QUEUE_H__