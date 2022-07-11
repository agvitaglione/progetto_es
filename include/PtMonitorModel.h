#ifndef __PTMONITORMODEL_H__
#define __PTMONITORMODEL_H__

#include <mqueue.h>
#include <string>
#include "TypeDefinitions.h"
#include "MessageQueueConcurrent.h"
#include "DataStore.h"
#include <thread>
#include <vector>


class PtMonitorModel {

    public:

        ~PtMonitorModel();
        
        //SINGLETON
        static PtMonitorModel* getInstance();

        // DISABLE OTHER CONSTRUCOTRS
        PtMonitorModel(const PtMonitorModel&) = delete;
        const PtMonitorModel& operator=(const PtMonitorModel&) = delete;

        bool getData(MessageType& message);

        // ROUTINE TO TAKE DATA FROM USB2CAN MODULE
        static void readDataFromModule();


        //---------------------------- USB FUNCTIONS

        /**
         * @brief Set the DataStore. Create a dataStore which can be used to write into an USB storage.
         * The path will be /home/{usbLabel}/ptmonitorLog.txt
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
         * @return std::vector<USB_t> List of USB
         */
        std::vector<USB_t> getUSBList() const;

        /**
         * @brief Moun the USB to /home/{USB_LABEL}
         * 
         * @param usb USB_t usb to be mounted
         * @param path Path where usb is mounting.
         * 
         * @return true if the operation succeded
         * @return false Error during the operation ha occured.
         */
        bool mountUSB(USB_t usb, std::string path);

    protected:
        PtMonitorModel();

    private:
        static MessageQueueConcurret queue;
        std::thread *readDataFromModuleThread; 

        // WRITE FILE LOG
        static DataStore dataStore;

};

#endif // __PTMONITORMODEL_H__