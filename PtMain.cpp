#include "PtMonitorView.h"
#include "PtMonitorControl.h"
#include <thread>
#include <chrono>

void task(PtMonitorView* view) {
    view->startRoutine();
}

int main() {

    PtMonitorView* view = PtMonitorView::getInstance();
    PtMonitorControl* control = PtMonitorControl::getInstance(view);

    std::thread t(task, view);
    
    while(true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        view->setMeasureValues(g_random_double()*100, PtMonitorView::TEMPERATURE, PtMonitorView::FL);
        view->setMeasureValues(g_random_double()*100, PtMonitorView::PRESSURE, PtMonitorView::FL);

        view->setMeasureValues(g_random_double()*100, PtMonitorView::TEMPERATURE, PtMonitorView::FR);
        view->setMeasureValues(g_random_double()*100, PtMonitorView::PRESSURE, PtMonitorView::FR);

        view->setMeasureValues(g_random_double()*100, PtMonitorView::TEMPERATURE, PtMonitorView::RL);
        view->setMeasureValues(g_random_double()*100, PtMonitorView::PRESSURE, PtMonitorView::RL);

        view->setMeasureValues(g_random_double()*100, PtMonitorView::TEMPERATURE, PtMonitorView::RR);
        view->setMeasureValues(g_random_double()*100, PtMonitorView::PRESSURE, PtMonitorView::RR);
    }
    
    t.join();

    return 0;
}