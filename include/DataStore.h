#ifndef __DATA_STORE_H__
#define __DATA_STORE_H__
#include <string>
#include <fstream>
#include <mutex>

class DataStore {

    public:
        /**
         * @brief Open log file setting the path where the log file will be saved. 
         *  The path will be: /home/{usbLabel}/ptmonitorLog.txt 
         *  The usb must have been mounted
         * 
         * @param usbLabel Label associated to USB storage
         */
        DataStore(std::string usbLabel);

        /**
         * @brief Close /home/{usbLabel}/ptmonitorLog.txt if it's opened
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
         * @brief Set the path where the log file will be saved and open ptmonitorLog.txt file
         *  The path will be: /home/{usbLabel}/ptmonitorLog.txt 
         * 
         *  The usb must have been mounted
         * 
         * @param usbLabel Label associated to USB storage
         */
        void setUsbLabel(std::string usbLabel);

        /**
         * @brief Check if /home/{usbLabel}/ptmonitorLog.txt is opened. 
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