#include "PtMonitorView.h"
#include "PtMonitorControl.h"
#include "PtMonitorModel.h"
#include <thread>
#include <chrono>
#include <iostream>
#include "DataPlotQueue.h"
#include "TypeDefinitions.h"

void task(PtMonitorView* view) {
    view->startRoutine();
}

int main() {


    PtMonitorView* view = PtMonitorView::getInstance();
    PtMonitorModel *model = PtMonitorModel::getInstance();
    PtMonitorControl* control = PtMonitorControl::getInstance(view, model);

    std::thread t(task, view);
    
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

    return 0;
}