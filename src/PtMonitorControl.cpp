#include "PtMonitorControl.h"
#include <chrono>
#include <thread>
#include "PtConfig.h"

// THREAD
static std::thread *periodicThread;

static int stopThread = 0;
static int naxis;
static int ntyre;

// MACRO
#define SWIPE_RIGHT(v_x, v_y) (v_x >= PtMonitorControl::XACT && abs(v_y) < PtMonitorControl::YLIMIT) // CHECK IT'S A RIGHT SWIPE
#define SWIPE_LEFT(v_x, v_y) (v_x <= -PtMonitorControl::XACT && abs(v_y) < PtMonitorControl::YLIMIT) // CHECK IT'S A LEFT SWIPE

// STATIC VARIABLES INITIALIZATION
PtMonitorView *PtMonitorControl::view = nullptr;
PtMonitorModel *PtMonitorControl::model = nullptr;
DataPlotQueue **PtMonitorControl::queues = nullptr;

/* PERIODIC TASK
* EACH X SECONDS, IT GETS DATA FROM MODEL
* IF THERE AREN'T NEW DATA, IT JUST UPDATES THE CURRENT PLOT
*/
void PtMonitorControl::periodicGetData() {

    MessageType message;
    MessageType messageArray[MAX_QUEUE_SIZE];
    int nelem;
    int currentTime;

    PtConfig *config = PtConfig::getInstance();
    naxis = config->getNumberOfAxis();
    ntyre = config->getNumberOfTyrePerAxis();
    DataType dataTemperature[naxis][ntyre];
    DataType dataPressure[naxis][ntyre];
    DataPlotQueue *queue;
    int axis;
    int tyre;

    while(stopThread == 0) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
            

        while(model->getData(message) == false);

        if(!config->isValidId(message.id)) continue;    // if it is not a valid id, ignore the message

        axis = config->getAxisFromId(message.id);
        tyre = config->getTyreFromId(message.id);
        queue = &queues[axis][tyre];
        queue->push(message);
        
        // PLOT DATA
        nelem = queue->getNelem();
        queue->getData(messageArray);
        

        for(int i = 0; i < nelem; i++) {
           dataTemperature[axis][tyre].x[i] = -i;
           dataTemperature[axis][tyre].y[i] = messageArray[i].temperature;

           dataPressure[axis][tyre].x[i] = -i;
           dataPressure[axis][tyre].y[i] = messageArray[i].pressure;
        }

        // PLOT DATA
        std::cout << "nelem: " << nelem << std::endl;
        view->plotData(dataTemperature[axis][tyre], nelem, TEMPERATURE, axis, tyre);
        view->plotData(dataPressure[axis][tyre], nelem, PRESSURE, axis, tyre);

        // SET LABELS
        view->setMeasureValues(message.temperature, TEMPERATURE, axis, tyre);
        view->setMeasureValues(message.pressure, PRESSURE, axis, tyre);

    }

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

    PtConfig *conf = PtConfig::getInstance();

    // CREATE QUEUES. ONE FOR EACH TYRE
    queues = new DataPlotQueue*[conf->getNumberOfAxis()];
    for(int i = 0; i < conf->getNumberOfAxis(); i++) {
        queues[i] = new DataPlotQueue[conf->getNumberOfTyrePerAxis()];
    }

    periodicThread = new std::thread(periodicGetData);
}

PtMonitorControl::~PtMonitorControl() {

    stopThread = 1;
    periodicThread->join();
    delete[] periodicThread;

    // DELETE QUEUES
    for(int i = 0; i < naxis; i++) {
        delete[] queues[i];
    }
    delete[] queues;
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