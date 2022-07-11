#include "MessageQueueConcurrent.h"

MessageQueueConcurret::MessageQueueConcurret(const int size) : MessageQueue(size) {
}

MessageQueueConcurret::MessageQueueConcurret(const MessageQueueConcurret& other) : MessageQueue(other) {
}


void MessageQueueConcurret::push(const MessageType& message) {
   mutex.lock();
   MessageQueue::push(message);
   mutex.unlock();
}

bool MessageQueueConcurret::pop(MessageType& message) {
    bool result;
    mutex.lock();
    result = MessageQueue::pop(message);
    mutex.unlock();
    return result;
}
