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
        * ID must be an array of dimension numberOfAxis x numberOfTyrePerAxis
        */
        void saveNewConfiguration(const int numberOfAxis, const int numberOfTyrePerAxis, std::string *id);

        int getNumberOfAxis() const; 
        int getNumberOfTyrePerAxis() const; 
        int getAxisFromId(const uint32_t id);
        int getTyreFromId(const uint32_t id);
        std::string getLogPath() const;
        bool isValidId(const uint32_t id) const;


    protected:
        PtConfig(void) {}

    private:

        // utile type 
        typedef struct {
            int axis;
            int tyre;
        } TyrePosition;

        std::fstream file;
        int numberOfAxis;
        int numberOfTyrePerAxis;
        std::string logPath;
        std::map<uint32_t, TyrePosition> positions; 

};



#endif // __PTCONFIG_H__