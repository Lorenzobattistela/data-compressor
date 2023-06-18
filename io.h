#ifndef IO_H
#define IO_H

#include <string>

using namespace std;

class IO {
    public:
        string filename;
        string text;
        long fileSize;

        IO (string filename);

        int readFile();

        void reportFileSize();

    private:
        long calculateFileSize();

};

#endif