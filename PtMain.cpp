#include "PtMonitorView.h"
#include "PtMonitorControl.h"
#include "PtMonitorModel.h"
#include <thread>
#include <chrono>
#include <iostream>
#include "DataPlotQueue.h"
#include "TypeDefinitions.h"
#include <mqueue.h>

void task(PtMonitorView* view) {
    view->startRoutine();
}

int main() {

    
    PtMonitorView* view = PtMonitorView::getInstance();
    PtMonitorModel *model = PtMonitorModel::getInstance();
    PtMonitorControl* control = PtMonitorControl::getInstance(view, model);
    
    
    std::thread t(task, view);
    /*
    while(true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        view->setMeasureValues(g_random_double()*100, MeasureType::TEMPERATURE, TyreType::FL);
        view->setMeasureValues(g_random_double()*100, MeasureType::PRESSURE, TyreType::FL);

        view->setMeasureValues(g_random_double()*100, MeasureType::TEMPERATURE, TyreType::FR);
        view->setMeasureValues(g_random_double()*100, MeasureType::PRESSURE, TyreType::FR);

        view->setMeasureValues(g_random_double()*100, MeasureType::TEMPERATURE, TyreType::RL);
        view->setMeasureValues(g_random_double()*100, MeasureType::PRESSURE, TyreType::RL);

        view->setMeasureValues(g_random_double()*100, MeasureType::TEMPERATURE, TyreType::RR);
        view->setMeasureValues(g_random_double()*100, MeasureType::PRESSURE, TyreType::RR);
    }
    
    t.join();
    */

    std::stringstream message;
    mqd_t queue;
    mq_attr attr;
    attr.mq_maxmsg = 61;
    attr.mq_flags = O_NONBLOCK;
     
    queue = mq_open("ptm_measure_queue", O_RDWR | O_CREAT, 0664, &attr);


    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        message << g_random_int_range(1000, 9999) <<  g_random_int_range(10, 99)
                << g_random_int_range(1000, 9999) <<  g_random_int_range(10, 99)
                << g_random_int_range(1000, 9999) <<  g_random_int_range(10, 99)
                << g_random_int_range(1000, 9999) <<  g_random_int_range(10, 99);

        mq_send(queue, message.str().c_str(), 24, 0);

        message.str("");

    }

    return 0;
}