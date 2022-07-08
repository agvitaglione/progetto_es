#ifndef __PTMONITORMODEL_H__
#define __PTMONITORMODEL_H__

#include <mqueue.h>
#include <string>
#include "TypeDefinitions.h"
#include "DataPlotQueueConcurrent.h"
#include "DataStore.h"
#include <thread>


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

        // SET DATASTORE
        void setDataStore(std::string path);


    protected:
        PtMonitorModel();

    private:
        static DataPlotQueueConcurret queue;
        std::thread *readDataFromModuleThread; 

        // WRITE FILE LOG
        static DataStore *dataStore;
};

#endif // __PTMONITORMODEL_H__