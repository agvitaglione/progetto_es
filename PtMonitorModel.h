#ifndef __PTMONITORMODEL_H__
#define __PTMONITORMODEL_H__

#include <mqueue.h>
#include <string>



class PtMonitorModel {


    public:

        static const int MAX_QUEUE_SIZE = 61;

        //SINGLETON
        static PtMonitorModel* getInstance();

        // DISABLE OTHER CONSTRUCOTRS
        PtMonitorModel(const PtMonitorModel&) = delete;
        const PtMonitorModel& operator=(const PtMonitorModel&) = delete;

        ~PtMonitorModel();

        // --------- TYPE DEFINITIONS

        typedef struct {
            float fl[2];
            float fr[2];
            float rl[2];
            float rr[2];
        } MessageType;

        typedef enum {
            TEMPERATURE,
            PRESSURE
        } MeasureType;

        // ---------

        // --------- UTILITY FUNCTIONS

        bool getData(MessageType& message) const;

        // ---------

        // --------- SET CALLBACK FUNCTIONS

        void setNewMassageHandler(void (*callback)(void));

        // ---------

    protected:
        PtMonitorModel();

    private:

        static const int MSG_SIZE = 24;
        inline static const std::string NAME_QUEUE = "ptm_measure_queue";

        mqd_t queue; 


        // --------- SET CALLBACK FUNCTIONS

        void (*newMessageHandler)(void);

        // ---------

        
};

#endif // __PTMONITORMODEL_H__