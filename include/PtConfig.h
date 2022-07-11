#ifndef __PTCONFIG_H__
#define __PTCONFIG_H__

#include <fstream>
#include <string>
#include <map>

class PtConfig {


    public:

        // SINGLETON
        static PtConfig* getInstance();
        ~PtConfig();

        // DISABLE OTHER CONSTRUCTORS
        PtConfig(const PtConfig&) = delete;
        const PtConfig& operator=(const PtConfig&) = delete; 

        void readFile(const std::string fileName);

        /*
        * ID must be an array of dimension numberOfAxis x numberOfTirePerAxis
        */
        void saveNewConfiguration(const int numberOfAxis, const int numberOfTirePerAxis, std::string *id);

        int getNumberOfAxis() const; 
        int getNumberOfTirePerAxis() const; 
        int getAxisFromId(const uint32_t id);
        int getTireFromId(const uint32_t id);
        bool isValidId(const uint32_t id) const;

        //CONNECTION VARIABLES
        std::string getRecIP() const;
        uint16_t getRecPort() const;


    protected:
        PtConfig(void) {}

    private:

        // utile type 
        typedef struct {
            int axis;
            int tire;
        } TirePosition;

        std::fstream file;
        int numberOfAxis;
        int numberOfTirePerAxis;
        std::map<uint32_t, TirePosition> positions; 

        // CONNECTION VARIABLES
        std::string recIP;
        uint16_t recPort;

};

#endif // __PTCONFIG_H__