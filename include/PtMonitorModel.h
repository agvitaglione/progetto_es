#ifndef __PTMONITORMODEL_H__
#define __PTMONITORMODEL_H__

#include <mqueue.h>
#include <string>
#include "TypeDefinitions.h"
#include "DataPlotQueueConcurrent.h"
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

        /**
         * @brief Set the DataStore 
         * 
         * @param usbLabel Label associated to the USB storage
         */
        void setDataStore(std::string usbLabel);

        //---------------------------- USB FUNCTIONS
        
        /**
         * @brief Get a list of currently plugged in data storages .
         * 
         * @return std::vector<USB_t> List of USB
         */
        std::vector<USB_t> getUSBList() const;

        /**
         * @brief Moun the USB to /home/{USERNAME}/{USB_LABEL}
         * 
         * @param usb USB_t usb to be mounted
         * 
         * @return bool true if the operation succeded
         */
        bool mountUSB(USB_t usb);

    protected:
        PtMonitorModel();

    private:
        static DataPlotQueueConcurret queue;
        std::thread *readDataFromModuleThread; 

        // WRITE FILE LOG
        static DataStore *dataStore;

};

#endif // __PTMONITORMODEL_H__