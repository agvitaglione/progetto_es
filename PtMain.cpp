#include "PtMonitorView.h"
#include "PtMonitorControl.h"

int main() {

    PtMonitorView* view = PtMonitorView::getInstance();
    PtMonitorControl* control = PtMonitorControl::getInstance(view);

    view->startRoutine();

    return 0;
}