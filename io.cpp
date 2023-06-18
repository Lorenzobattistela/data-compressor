#include "io.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sys/stat.h>

using namespace std;

IO::IO(string filename) {
    this->filename = filename;
    this->fileSize = calculateFileSize();
}

int IO::readFile() {
    ifstream File(this->filename);
    if(!File) {
        cout << "Error: File does not exist" << endl;
        return 1;
    }

    while(getline(File, this->text)) {
        this->fileSize++;
    }
    return 1;
}

void IO::reportFileSize() {
    string size = to_string(this->fileSize);
    cout << "File size: " << size << " bytes" << endl;
}

long IO::calculateFileSize() {
    struct stat stat_buf;
    int rc = stat(this->filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}