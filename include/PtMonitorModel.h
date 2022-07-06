#ifndef __PTMONITORMODEL_H__
#define __PTMONITORMODEL_H__

#include <mqueue.h>
#include <string>
#include "TypeDefinitions.h"
#include "DataPlotQueueConcurrent.h"
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


    protected:
        PtMonitorModel();

    private:
        static DataPlotQueueConcurret queue;
        std::thread *readDataFromModuleThread;
 
};

#endif // __PTMONITORMODEL_H__