#include "PtMonitorModel.h"
#include <signal.h>

MessageType messageParsing(const char msg[]) {

    // Message: TTTTPP * 4

    MessageType msgResult;
    std::string msgString(msg);

    msgResult.fl[MeasureType::TEMPERATURE] = stof(msgString.substr(0, 3) + "." + msgString.substr(3, 1));
    msgResult.fl[MeasureType::PRESSURE] = stof(msgString.substr(4, 1) + "." + msgString.substr(5, 1));

    msgResult.fr[MeasureType::TEMPERATURE] = stof(msgString.substr(6, 3) + "." + msgString.substr(9, 1));
    msgResult.fr[MeasureType::PRESSURE] = stof(msgString.substr(10, 1) + "." + msgString.substr(11, 1));  

    msgResult.rl[MeasureType::TEMPERATURE] = stof(msgString.substr(12, 3) + "." + msgString.substr(15, 1));
    msgResult.rl[MeasureType::PRESSURE] = stof(msgString.substr(16, 1) + "." + msgString.substr(17, 1));  

    msgResult.rr[MeasureType::TEMPERATURE] = stof(msgString.substr(18, 3) + "." + msgString.substr(21, 1));
    msgResult.rr[MeasureType::PRESSURE] = stof(msgString.substr(22, 1) + "." + msgString.substr(23, 1));  

    return msgResult;
}

PtMonitorModel* PtMonitorModel::getInstance() {

    static PtMonitorModel model = PtMonitorModel();
    return &model;

}

PtMonitorModel::PtMonitorModel() {

    mq_attr attr;
    attr.mq_maxmsg = MAX_QUEUE_SIZE;
    attr.mq_flags = O_NONBLOCK;
     
    queue = mq_open(NAME_QUEUE.c_str(), O_CREAT | O_RDONLY, 0664, &attr);
}

PtMonitorModel::~PtMonitorModel() {
    mq_unlink(NAME_QUEUE.c_str());
    mq_close(queue);
}

bool PtMonitorModel::getData(MessageType& message) const {
      
    char msg[MSG_SIZE]; 

    if(mq_receive(queue, msg, MSG_SIZE * sizeof(char), 0) != -1) {

        message = messageParsing(msg);
        return true;

    }

    return false;
}