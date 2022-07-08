#ifndef __DATA_STORE_H__
#define __DATA_STORE_H__
#include <string>
#include <fstream>
#include <mutex>

class DataStore {

    public:
        DataStore(std::string path);
        ~DataStore();
        void write(std::string line);
        void setPath(std::string path);

    private:
        std::fstream file;
        std::mutex mutex;
};

#endif // __DATA_STORE_H__