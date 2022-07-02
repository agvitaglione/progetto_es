#include "PtMonitorView.h"
#include "PtMonitorControl.h"
#include "PtMonitorModel.h"
#include <thread>
#include <chrono>
#include <iostream>
#include "DataPlotQueue.h"
#include "TypeDefinitions.h"
#include <mqueue.h>
#include <string.h>

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
    int queue = msgget(ftok("ptm_measure_queue", 0), IPC_CREAT | 0644);

    Msgbuf msg;

    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        message << g_random_int_range(1000, 9999) <<  g_random_int_range(10, 99)
                << g_random_int_range(1000, 9999) <<  g_random_int_range(10, 99)
                << g_random_int_range(1000, 9999) <<  g_random_int_range(10, 99)
                << g_random_int_range(1000, 9999) <<  g_random_int_range(10, 99) << "\0";

        msg.mtype = 1;
        strcpy(msg.mtext, message.str().c_str());
        message.str("");

        msgsnd(queue, &msg, sizeof(Msgbuf) - sizeof(long), 0);

    }

    return 0;
}