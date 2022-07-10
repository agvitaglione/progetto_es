
#include "PtMonitorView.h"
#include "PtMonitorControl.h"
#include "PtMonitorModel.h"
#include "PtConfig.h"



int main() {

    PtConfig *ptconfig = PtConfig::getInstance();
    ptconfig->readFile("../config.txt");

    PtMonitorView *view = PtMonitorView::getInstance();
    PtMonitorModel *model = PtMonitorModel::getInstance();
    PtMonitorControl *control = PtMonitorControl::getInstance(view, model);
    
    
    if(ptconfig->getRecIP() != "") {
        control->setConnection(ptconfig->getRecIP(), ptconfig->getRecPort());
    }

    view->startRoutine();


    return 0;
}