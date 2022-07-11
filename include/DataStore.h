#ifndef __DATA_STORE_H__
#define __DATA_STORE_H__
#include <string>
#include <fstream>
#include <mutex>

class DataStore {

    public:
        /**
         * @brief Open log file setting the path where the log will be saved. 
         * The usb must have been mounted
         * 
         * @param usbLabel Label associated to USB storage
         */
        DataStore(std::string usbLabel);

        /**
         * @brief Destructor. Close file if it's opened.
         * 
         */
        ~DataStore();

        /**
         * @brief Write the log line into an USB storage
         * 
         * @param line Line to be written.
         */
        void write(std::string line);

        /**
         * @brief Set the path/file.txt where to save logs.
         * 
         *  The usb must have been mounted
         * 
         * @param usbLabel Label associated to USB storage
         */
        void setPath(std::string path);

        /**
         * @brief Check if {path}/ptmonitorLog.txt is opened. 
         * 
         * @return true 
         * @return false 
         */
        bool isOpen() const;

        /**
         * @brief Close the file.
         *  You don't have to call closeFile before destroying the object.
         * 
         */
        void closeFile();

    private:
        std::fstream file;
        std::mutex mutex;
};
#endif // __DATA_STORE_H__