#ifndef IO_H
#define IO_H

#include <string>
#include <fstream>

using namespace std;

class IO {
    public:
        string filename;
        string text;
        long fileSize;

        IO (string filename);

        int readFile();

        void reportFileSize(string filename);

        streampos getBinaryFileSize(const string& filename);

    private:
        long calculateFileSize(string filename);

};

#endif