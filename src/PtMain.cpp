#include "PtMonitorView.h"
#include "PtMonitorControl.h"
#include "PtMonitorModel.h"
#include <thread>
#include <iostream>
#include "DataPlotQueue.h"
#include "TypeDefinitions.h"
#include "PtConfig.h"

int main() {

    
    PtConfig *ptconfig = PtConfig::getInstance();
    ptconfig->readFile("../config.txt");

    PtMonitorView *view = PtMonitorView::getInstance();
    PtMonitorModel *model = PtMonitorModel::getInstance();
    PtMonitorControl *control = PtMonitorControl::getInstance(view, model);

    // SET LOG
    /*
    if(ptconfig->getLogPath() != "") {
        model->setDataStore(ptconfig->getLogPath());
    }*/

    view->startRoutine();

    return 0;
}