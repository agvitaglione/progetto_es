#include "PtMonitorModel.h"
#include <signal.h>
#include <time.h>
#include <iomanip>

void messageParsing(const char msg[], MessageType& msgResult) {

    // Message: TTTTPP * 4
    std::string msgString(msg);
    
    msgResult.fl[MeasureType::TEMPERATURE] = std::stof(msgString.substr(0, 3) + ',' + msgString.substr(3, 1));
    msgResult.fl[MeasureType::PRESSURE] = stof(msgString.substr(4, 1) + "," + msgString.substr(5, 1));

    msgResult.fr[MeasureType::TEMPERATURE] = stof(msgString.substr(6, 3) + "," + msgString.substr(9, 1));
    msgResult.fr[MeasureType::PRESSURE] = stof(msgString.substr(10, 1) + "," + msgString.substr(11, 1));  

    msgResult.rl[MeasureType::TEMPERATURE] = stof(msgString.substr(12, 3) + "," + msgString.substr(15, 1));
    msgResult.rl[MeasureType::PRESSURE] = stof(msgString.substr(16, 1) + "," + msgString.substr(17, 1));  

    msgResult.rr[MeasureType::TEMPERATURE] = stof(msgString.substr(18, 3) + "," + msgString.substr(21, 1));
    msgResult.rr[MeasureType::PRESSURE] = stof(msgString.substr(22, 1) + "," + msgString.substr(23, 1));  
}

PtMonitorModel* PtMonitorModel::getInstance() {

    static PtMonitorModel model = PtMonitorModel();
    return &model;

}

PtMonitorModel::PtMonitorModel() {
    queue = msgget(ftok(NAME_QUEUE.c_str(), 0), IPC_CREAT | 0644);
}

PtMonitorModel::~PtMonitorModel() {
    msgctl(queue, IPC_RMID, 0);
}

bool PtMonitorModel::getData(MessageType& message) const {

    Msgbuf msg;
    
    if(msgrcv(queue, &msg, sizeof(Msgbuf) - sizeof(long), 1, IPC_NOWAIT) >= 0) {
        message.time = (int) time(NULL);
        messageParsing(msg.mtext, message);
       
        return true;

    }

    return false;
}