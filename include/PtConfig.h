#ifndef __PTCONFIG_H__
#define __PTCONFIG_H__
#include <fstream>
#include <string>
#include <map>

class PtConfig {
    public:

        /**
         * @brief Get the Instance of PtConfig
         * 
         * @return PtConfig* 
         */
        static PtConfig* getInstance();

        /**
         * @brief Destroy the Pt Config object
         * If file is open, close it 
         */
        ~PtConfig();

        /**
         * @brief Read the configuration file to set the internal variables.
         * It must be called before using any get.
         * The configiguration file must be placed in the root folder of the project.
         * 
         * @param fileName Configuration file's name.
         */
        void readFile(const std::string fileName);

        /**
         * @brief Save the new configuration in the configuration file.
         * You must have already called readFile before using this method.
         * 
         * id must be an array of dimension numberOfAxis x numberOfTirePerAxis sorted as follow.
         *            
         *    _____________FRONT___________
         *    |                           |
         *    |                           |
         *    |0_1_____________________2_3|
         *    |                           |
         *    |                           |
         *    |4_6_____________________6_7|
         *    |                           |
         *
         * id NONE means that there's no sensor placed in the tire, or there's no tire in that position.
         * 
         * WARNING: do not use this method since it's not fully implemented.
         * 
         * @param numberOfAxis 
         * @param numberOfTirePerAxis 
         * @param id Sensors' id. 
         */
        void saveNewConfiguration(const int numberOfAxis, const int numberOfTirePerAxis, std::string *id);

        /**
         * @brief Get the number ff Axis
         * 
         * @return int 
         */
        int getNumberOfAxis() const; 

        /**
         * @brief Get the number of tire per axis
         * 
         * @return int 
         */
        int getNumberOfTirePerAxis() const; 

        /**
         * @brief Get the axis given the sensor's id.
         * 
         * @see saveNewConfiguration
         * 
         * @param id Sensor's id
         * @return int Axis
         */
        int getAxisFromId(const uint32_t id);

        /**
         * @brief Get the tire given the sensor's id.
         * 
         * @see saveNewConfiguration
         * 
         * @param id Sensor's id
         * @return int Tire
         */
        int getTireFromId(const uint32_t id);

        /**
         * @brief Check that the id is valid (i.e. is written in the configuration file)
         * 
         * @param id Id to be checked
         * @return true The id is in the configuration file.
         * @return false The id isn't in the configuration file.
         */
        bool isValidId(const uint32_t id) const;
 
        /**
         * @brief Get the receiver IP
         * 
         * @return std::string 
         */
        std::string getRecIP() const;

        /**
         * @brief Get the receiver port
         * 
         * @return uint16_t 
         */
        uint16_t getRecPort() const;

        //------------------------------------- DISABLED METHODS

        /**
         * @brief The copy constructor is disabled.
         */
        PtConfig(const PtConfig&) = delete;

        /**
         * @brief The assignement operator is disabled.
         */
        const PtConfig& operator=(const PtConfig&) = delete; 

        //-------------------------------------

    protected:

        /**
         * @brief Construct a new Pt Config object.
         * Nothing to do.
         */
        PtConfig(void) {}

    private:

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