#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <string>
#include "cli.h"
#include "../io.h"
#include "../compressor/compressor.h"

using namespace std;

int main(int argc, char* argv[]) {
    CLI cli(argv);
    if(cli.checkArgs() == 1) {
        return 1;
    }

    IO io(cli.args[1]);
    io.reportFileSize(io.filename);
    io.readFile();

    cout << io.text << endl;

    Compressor compressor(io.text);
    string compressedText = compressor.compress();
    cout << compressedText << endl;

    ofstream outputFile("compressed.bin", ios::binary);
    if(outputFile) {
        vector<bool> compressedBits;
        for(char bit : compressedText) {
            compressedBits.push_back(bit == '1');
        }

        size_t numBytes = compressedBits.size() / 8;
        if (compressedBits.size() % 8 != 0) {
            numBytes += 1;
        }

        for (size_t i = 0; i < numBytes; i++) {
            unsigned char byte = 0;
            for (int j = 0; j < 8; j++) {
                if (i * 8 + j < compressedBits.size()) {
                    byte |= (compressedBits[i * 8 + j] ? (1 << j) : 0);
                }
            }
            outputFile.write(reinterpret_cast<const char*>(&byte), sizeof(byte));
        }
        outputFile.close();

        streampos binFileSize = io.getBinaryFileSize("compressed.bin");

        if (binFileSize != -1) {
            compressor.reportCompression(io.fileSize, binFileSize);
        } else {
            cout << "Failed to determine the size of the compressed file." << endl;
        }
    }

    else {
        cout << "Error: Failed to open file" << endl;
        return 1;
    }
}

CLI::CLI(char *args[]) {
    int i = 0;
    while (args[i] != nullptr) {
        this->args.push_back(args[i]);
        i++;
    }
}

int CLI::checkArgs() {
    if(this->args.size() < 2) {
        cout << "Usage: " << this->args[0] << " <filename>" << endl;
        return 1;
    }

    if(this->args[1].substr(this->args[1].length() - 4, this->args[1].length()) != ".txt") {
        cout << "Error: File must be a text file (.txt)" << endl;
        return 1;
    }

    return 0;
}