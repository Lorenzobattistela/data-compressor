#include "io.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <sys/stat.h>

using namespace std;

IO::IO(string filename) {
    this->filename = filename;
    this->fileSize = calculateFileSize(filename);
}

int IO::readFile() {
    ifstream file(this->filename);
    if(!file) {
        cout << "Error: File does not exist" << endl;
        return 1;
    }

    stringstream buffer;
    buffer << file.rdbuf();
    this->text = buffer.str();
    return 0;
}

void IO::reportFileSize(string filename) {
    calculateFileSize(filename);
    string size = to_string(this->fileSize);
    cout << "File size: " << size << " bytes" << endl;
}

long IO::calculateFileSize(string filename) {
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

streampos IO::getBinaryFileSize(const string& filename) {
    ifstream file(filename, ios::binary | ios::ate);
    if (file) {
        return file.tellg();
    } else {
        return -1;
    }
}