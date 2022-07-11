#ifndef __PTMONITORMODEL_H__
#define __PTMONITORMODEL_H__

#include <string>
#include "TypeDefinitions.h"
#include "MessageQueueConcurrent.h"
#include "DataStore.h"
#include <thread>
#include <vector>

class PtMonitorModel {

    public:

        /**
         * @brief Get the Instance object.
         * 
         * @return PtMonitorModel* 
         */
        static PtMonitorModel* getInstance();

        /**
         * @brief Destroy the Pt Monitor Model object.
         * 
         */
        ~PtMonitorModel();

        /**
         * @brief The copy constructor is disabled.
         * 
         */
        PtMonitorModel(const PtMonitorModel&) = delete;

        /**
         * @brief The assignment operator is deleted. 
         * 
         */
        const PtMonitorModel& operator=(const PtMonitorModel&) = delete;

        /**
         * @brief Pop the oldest message from the queue. 
         * 
         * @param[out] message 
         * @return true if there is a massage to pop.
         * @return false if there isn't any massage to pop.
         */
        bool getData(MessageType& message);

        /**
         * @brief Routine that in polling read messages from CAN bus. New messages are pushed in the queue.
         *  If a data store is open, the messages are written in a log file. 
         *  It starts when the object is costructed.
         * 
         */
        static void readDataFromModule();

        // ---------------------------- USB FUNCTIONS

        /**
         * @brief Set the DataStore. Create a dataStore which can be used to write into an USB storage.
         *  The path will be /home/{usbLabel}/ptmonitorLog.txt
         * 
         * @param usbLabel Label associated to the USB storage
         */
        void setDataStore(std::string usbLabel);

        /**
         * @brief Check if the DataStore has been set
         * 
         * @return true 
         * @return false 
         */
        bool isDataStoreSet() const;

        /**
         * @brief Delete DataStore. It disables the logging activity.
         * 
         */
        void deleteDataStore();

        /**
         * @brief Get a list of currently plugged in data storages .
         * 
         * @return std::vector<USB_t> List of USB plugged in.
         */
        std::vector<USB_t> getUSBList() const;

        /**
         * @brief Moun the USB to /home/{USB_LABEL}
         * 
         * @param usb USB_t usb to be mounted
         * @param path Path where usb is mounting.
         * 
         * @return true if the operation succeded
         * @return false Error during the operation has occured.
         */
        bool mountUSB(USB_t usb, std::string path);

        //----------------------------

    protected:

        /**
         * @brief Construct a new Pt Monitor Model object.
         *  It runs readDataFromModule task.
         * 
         * @see readDataFromModule
         * 
         */
        PtMonitorModel();

    private:

        /// Queue where all messages sent are stored. 
        static MessageQueueConcurret queue;

        /**
         * @brief Indipendent task which read messages sent through can bus.
         * 
         * @see readDataFromModule
         */
        std::thread *readDataFromModuleThread; 

        /// Object used for message logging.
        static DataStore dataStore;

};

#endif // __PTMONITORMODEL_H__