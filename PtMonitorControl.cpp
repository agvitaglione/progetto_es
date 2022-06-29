#include "PtMonitorControl.h"

// MACRO
#define SWIPE_RIGHT(v_x, v_y) (v_x >= PtMonitorControl::XACT && abs(v_y) < PtMonitorControl::YLIMIT) // CHECK IT'S A RIGHT SWIPE
#define SWIPE_LEFT(v_x, v_y) (v_x <= -PtMonitorControl::XACT && abs(v_y) < PtMonitorControl::YLIMIT) // CHECK IT'S A LEFT SWIPE

PtMonitorControl::PtMonitorControl(PtMonitorView* view) {
    this->view = view;
    this->view->setShoutdownHandler((void*)&shutdownHandler);
    this->view->setSwipeHandler((void*)&swipeHandler);
}

void PtMonitorControl::shutdownHandler(void) {
    std::cout << "Chiudere!" << std::endl;
}

void PtMonitorControl::swipeHandler(GtkGestureSwipe *swipe, gdouble v_x, gdouble v_y) {
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

    view->setPage(currentPageNumber, transition);

}