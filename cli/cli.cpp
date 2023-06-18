#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <string>
#include "cli.h"
#include "../io.h"
#include "../compressor/compressor.h"
#include "../decompressor/decompressor.h"

using namespace std;

int main(int argc, char* argv[]) {
    CLI cli(argv);
    if(cli.checkArgs() == 1) {
        return 1;
    }
    
    bool compress = cli.args[2] == "-c";

    if(compress) {
        IO io(cli.args[1]);
        io.reportFileSize(io.filename);
        io.readFile();

        cout << io.text << endl;

        Compressor compressor(io.text);
        string compressedText = compressor.compress();
        compressor.printHuffmanCodes();
        cout << compressedText << endl;

        ofstream outputFile("compressed.bin", ios::binary);
        if(outputFile) {
            vector<unsigned char> compressedBytes;
            for (size_t i = 0; i < compressedText.length(); i += 8) {
                unsigned char byte = 0;
                for (size_t j = 0; j < 8; j++) {
                    if (i + j < compressedText.length()) {
                        byte |= (compressedText[i + j] - '0') << (7 - j);
                    }
                }
                compressedBytes.push_back(byte);
            }
            outputFile.write(reinterpret_cast<const char*>(compressedBytes.data()), compressedBytes.size());
            outputFile.close();

            streampos binFileSize = io.getBinaryFileSize("compressed.bin");

            if (binFileSize != -1) {
                compressor.reportCompression(io.fileSize, binFileSize);
    
                compressor.writeFrequencyTable();

            } else {
                cout << "Failed to determine the size of the compressed file." << endl;
            }
        }

        else {
            cout << "Error: Failed to open file" << endl;
            return 1;
        }
    }

    else {
        if(cli.args.size() < 3) {
            cout << "Missing file names" << endl;
            return 1;
        }
        Decompressor decompressor(cli.args[1], cli.args[2]);
        decompressor.decompress();
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
    if(this->args.size() < 3) {
        cout << "Usage: " << this->args[0] << " <filename> -c (to compress)" << endl;
        cout << "Usage: " << this->args[0] << " <binary compreessed filename> <output filename> to decompress" << endl;
        return 1;
    }

    return 0;
}