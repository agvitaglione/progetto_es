#include "PtMonitorView.h"
#include "PtMonitorControl.h"

int main() {

    PtMonitorView view;
    PtMonitorControl control(&view);

    view.startRoutine();

    return 0;
}