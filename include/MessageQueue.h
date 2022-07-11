#ifndef __DATA_PLOT_QUEUE_H__
#define __DATA_PLOT_QUEUE_H__
#include "TypeDefinitions.h"

class MessageQueue {

    public:

        /**
         * @brief Construct a new Data Plot Queue object.
         * 
         * @param size The size of the queue.
         */
        MessageQueue(const int size = MAX_QUEUE_SIZE);

        /**
         * @brief Copy constructor
         * 
         * @param other 
         */
        MessageQueue(const MessageQueue& other);

        /**
         * @brief Destroy the Message Queue object.
         * 
         */
        ~MessageQueue();

        /**
         * @brief Insert a message into the queue.
         * If the queue is full, the new message replaces the oldest one.
         * 
         * @param message The message to push in the queue.
         */
        virtual void push(const MessageType& message);

        /**
         * @brief Pop a message out the queue.
         * 
         * @param[out] message 
         * @return true The queue was not empty.
         * @return false The queue is empty.
         */
        virtual bool pop(MessageType& message);

        /**
         * @brief Get an array of messages sorted chronologically (i.e. from head to tail).
         * 
         * @param[out] messageArray 
         */
        void getData(MessageType *messageArray) const;

        /**
         * @brief Get the number of element currently stored in the queue.
         * 
         * @return int 
         */
        int getNelem() const {return nelem;}

        /**
         * @brief Get the queue size.
         * 
         * @return int 
         */
        int getSize() const {return size;}

    private:

	    MessageType *queue;
        int head;
        int tail;
        int nelem;
        int size;
};
#endif // __DATA_PLOT_QUEUE_H__