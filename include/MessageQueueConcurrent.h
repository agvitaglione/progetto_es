#ifndef __DATA_PLOT_QUEUE_CONCURRET_H__
#define __DATA_PLOT_QUEUE_CONCURRET_H__
#include "MessageQueue.h"
#include "TypeDefinitions.h"
#include <mutex>

class MessageQueueConcurret : public MessageQueue {

    public:

        /**
         * @brief Construct a new Message Queue Concurret object.
         * 
         * @param size The size of the queue.
         */
        MessageQueueConcurret(const int size);

        /**
         * @brief Copy constructor.
         * 
         * @param other 
         */
        MessageQueueConcurret(const MessageQueueConcurret& other);

        /**
         * @brief Insert a message into the queue.
         * If the queue is full, the new message replaces the oldest one.
         * 
         * This method is thread safe.
         * 
         * @param message The message to push in the queue.
         */
        virtual void push(const MessageType& message);

        /**
         * @brief Pop a message out the queue.
         * 
         * This method is thread safe.
         * 
         * @param[out] message 
         * @return true The queue was not empty.
         * @return false The queue is empty.
         */
        virtual bool pop(MessageType& message);

    private:
        std::mutex mutex;
};
#endif //__DATA_PLOT_QUEUE_CONCURRET_H__