#ifndef __PTMONITORMODEL_H__
#define __PTMONITORMODEL_H__

#include <mqueue.h>
#include <string>
#include "TypeDefinitions.h"



class PtMonitorModel {


    public:

        static const int MAX_QUEUE_SIZE = 61;

        //SINGLETON
        static PtMonitorModel* getInstance();

        // DISABLE OTHER CONSTRUCOTRS
        PtMonitorModel(const PtMonitorModel&) = delete;
        const PtMonitorModel& operator=(const PtMonitorModel&) = delete;

        ~PtMonitorModel();



        bool getData(MessageType& message) const;

        // ---------

    protected:
        PtMonitorModel();

    private:

        static const int MSG_SIZE = 24;
        inline static const std::string NAME_QUEUE = "ptm_measure_queue";

        // POSIX QUEUE
        mqd_t queue; 
 
};

#endif // __PTMONITORMODEL_H__