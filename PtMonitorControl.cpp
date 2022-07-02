#include "PtMonitorControl.h"
#include <chrono>
#include <thread>

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

    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        while(model->getData(message)) {
            
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
    this->view->setShoutdownHandler(&PtMonitorControl::shutdownHandler);
    this->view->setSwipeHandler(&PtMonitorControl::swipeHandler);
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