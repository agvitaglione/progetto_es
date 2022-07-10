#include "PtMonitorControl.h"
#include <chrono>
#include <thread>
#include "PtConfig.h"
#include "RemoteConnection.h"

// MACRO
#define SWIPE_RIGHT(v_x, v_y) (v_x >= PtMonitorControl::XACT && abs(v_y) < PtMonitorControl::YLIMIT) // CHECK IT'S A RIGHT SWIPE
#define SWIPE_LEFT(v_x, v_y) (v_x <= -PtMonitorControl::XACT && abs(v_y) < PtMonitorControl::YLIMIT) // CHECK IT'S A LEFT SWIPE

// REMOTE CONNECTION
static RemoteConnection rc;

// THREAD
static std::thread *periodicThread;
static int stopThread = 0;
static int naxis;
static int ntyre;

// STATIC VARIABLES INITIALIZATION
PtMonitorView *PtMonitorControl::view = nullptr;
PtMonitorModel *PtMonitorControl::model = nullptr;
DataPlotQueue **PtMonitorControl::queues = nullptr;


// SENDDATA TO REMOTE COMPUTER
void sendData(const MessageType& data) {
    std::string message = "[" + std::to_string(data.time) + "] id-sensor: " + std::to_string(data.id) + " temperature: " 
                        + std::to_string(data.temperature) + " pressure: " + std::to_string(data.pressure);
    rc.write(message);
}

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
           dataTemperature[axis][tyre].x[i] = -nelem + i + 1;
           dataTemperature[axis][tyre].y[i] = messageArray[i].temperature;

           dataPressure[axis][tyre].x[i] = -nelem + i + 1;
           dataPressure[axis][tyre].y[i] = messageArray[i].pressure;
        }

        // PLOT DATA
        view->plotData(dataTemperature[axis][tyre], nelem, TEMPERATURE, axis, tyre);
        view->plotData(dataPressure[axis][tyre], nelem, PRESSURE, axis, tyre);
        
        //SET LABELS
        view->setMeasureValues(message.temperature, TEMPERATURE, axis, tyre);
        view->setMeasureValues(message.pressure, PRESSURE, axis, tyre);
        
        // SEND DATA TO REMOTE
        if(rc.isConnected()) {
            sendData(message);
        }
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

    // SET HANDLER
    this->view->setShoutdownHandler(&PtMonitorControl::shutdownHandler);
    this->view->setSwipeHandler(&PtMonitorControl::swipeHandler);
    this->view->setUsbButtonHandler(&PtMonitorControl::usbHandler);
    this->view->setUsbReleaseButtonHandler(&PtMonitorControl::usbReleaseHandler);
    this->view->setUsbReloadButtonHandler(&PtMonitorControl::usbReloadHandler);

    PtConfig *conf = PtConfig::getInstance();

    // CREATE QUEUES. ONE FOR EACH TYRE
    queues = new DataPlotQueue*[conf->getNumberOfAxis()];
    for(int i = 0; i < conf->getNumberOfAxis(); i++) {
        queues[i] = new DataPlotQueue[conf->getNumberOfTyrePerAxis()];
    }

    // PERIODIC TASK WHICH GET DATA FROM MODEL QUEUE
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
    system("shutdown now");
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

void PtMonitorControl::setConnection(std::string recIP, uint16_t recPort) {
    rc.setConnection(recIP, recPort);
}

void PtMonitorControl::usbReloadHandler(void) {
    std::vector<USB_t> usbList = model->getUSBList();
    view->removeAllUSB();

    for(auto usb : usbList) {
        view->addUSB(usb.label);
    }
}


void PtMonitorControl::usbReleaseHandler(void) {
    model->deleteDataStore();
}


void PtMonitorControl::usbHandler(std::string usbLabel) {

    std::vector<USB_t> vector = model->getUSBList();

    for(auto usb : vector) {
        if(usb.label == usbLabel) {
            model->mountUSB(usb);
            break;
        }
    }

    model->setDataStore(usbLabel);
}
