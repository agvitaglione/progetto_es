#include "PtMonitorControl.h"
#include <chrono>
#include <thread>

// THREAD
std::thread *periodicThread;
DataType dataTemperature[4];
DataType dataPressure[4];
int stopThread = 0;

// MACRO
#define SWIPE_RIGHT(v_x, v_y) (v_x >= PtMonitorControl::XACT && abs(v_y) < PtMonitorControl::YLIMIT) // CHECK IT'S A RIGHT SWIPE
#define SWIPE_LEFT(v_x, v_y) (v_x <= -PtMonitorControl::XACT && abs(v_y) < PtMonitorControl::YLIMIT) // CHECK IT'S A LEFT SWIPE

// STATIC VARIABLES INITIALIZATION
PtMonitorView* PtMonitorControl::view = nullptr;
PtMonitorModel* PtMonitorControl::model = nullptr;
DataPlotQueue PtMonitorControl::queue(PtMonitorModel::MAX_QUEUE_SIZE);

/* PERIODIC TASK
* EACH X SECONDS, IT GETS DATA FROM MODEL
* IF THERE AREN'T NEW DATA, IT JUST UPDATES THE CURRENT PLOT
*/
void PtMonitorControl::periodicGetData() {

    MessageType message;
    int size = queue.getSize();
    MessageType messageArray[size];
    int nelem;

    for(int i = 0; i < 4; i++) {
        // QUANDO DEALLOCARE?
        dataTemperature[i].x = new double[size];
        dataTemperature[i].y = new double[size];
        dataPressure[i].x = new double[size];
        dataPressure[i].y = new double[size];
    }

    dataTemperature[FL].tyre = FL;
    dataTemperature[FR].tyre = FR;
    dataTemperature[RL].tyre = RL;
    dataTemperature[RR].tyre = RR;

    dataPressure[FL].tyre = FL;
    dataPressure[FR].tyre = FR;
    dataPressure[RL].tyre = RL;
    dataPressure[RR].tyre = RR;

    while(stopThread == 0) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        while(model->getData(message)) {
            queue.push(message);
        }

        // PLOT DATA
        nelem = queue.getNelem();
        queue.getData(messageArray);

        for(int i = 0; i < nelem; i++) {

            // TEMPERATURE
            dataTemperature[FL].y[i] = messageArray[i].fl[TEMPERATURE];
            dataTemperature[FL].x[i] = messageArray[i].time;

            dataTemperature[FR].y[i] = messageArray[i].fr[TEMPERATURE];
            dataTemperature[FR].x[i] = messageArray[i].time;        

            dataTemperature[RL].y[i] = messageArray[i].rl[TEMPERATURE];
            dataTemperature[RL].x[i] = messageArray[i].time;
            
            dataTemperature[RR].y[i] = messageArray[i].rr[TEMPERATURE];
            dataTemperature[RR].x[i] = messageArray[i].time;
        
            // PRESSURE
            dataPressure[FL].y[i] = messageArray[i].fl[PRESSURE];
            dataPressure[FL].x[i] = messageArray[i].time;
            
            dataPressure[FR].y[i] = messageArray[i].fr[PRESSURE];
            dataPressure[FR].x[i] = messageArray[i].time;
            
            dataPressure[RL].y[i] = messageArray[i].rl[PRESSURE];
            dataPressure[RL].x[i] = messageArray[i].time;
            
            dataPressure[RR].y[i] = messageArray[i].rr[PRESSURE];
            dataPressure[RR].x[i] = messageArray[i].time;
            
        }
    }

    view->plotData(dataTemperature, TEMPERATURE, nelem);
    view->plotData(dataTemperature, PRESSURE, nelem);
    
}



// SINGLETON GET INSTANCE
PtMonitorControl* PtMonitorControl::getInstance(PtMonitorView* view, PtMonitorModel* model) {
    static PtMonitorControl instance(view, model);
    return &instance;
}

// CONSTRUCTOR
PtMonitorControl::PtMonitorControl(PtMonitorView* _view, PtMonitorModel* _model) {
    PtMonitorControl::view = _view;
    PtMonitorControl::model = _model;
    this->view->setShoutdownHandler(&PtMonitorControl::shutdownHandler);
    this->view->setSwipeHandler(&PtMonitorControl::swipeHandler);

    periodicThread = new std::thread(periodicGetData);
}

PtMonitorControl::~PtMonitorControl() {

    stopThread = 1;
    periodicThread->join();
    delete[] periodicThread;
    
    for(int i = 0; i < 4; i++) {
        delete[] dataTemperature[i].x;
        delete[] dataTemperature[i].y;
    }
}


void PtMonitorControl::shutdownHandler(void) {
    // TODO
    //std::cout << "Chiudere!" << std::endl;
}

void PtMonitorControl::swipeHandler(gdouble v_x, gdouble v_y) {
    int pagesNumber = PtMonitorView::getNumberOfPages();
    int currentPageNumber = view->getCurrentPageNumber();
    int nextPage = currentPageNumber;
    GtkStackTransitionType transition;

    if(SWIPE_RIGHT(v_x, v_y) && currentPageNumber > 1)  {
        nextPage--;
        transition = GTK_STACK_TRANSITION_TYPE_SLIDE_RIGHT;
    }

    else if (SWIPE_LEFT(v_x, v_y) && currentPageNumber < pagesNumber)  {
        nextPage++;
        transition = GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT;
    }

    view->setPage(nextPage, transition);

}

void PtMonitorControl::newMessageHandler() {
    //TODO

}