#ifndef DECOMPRESSOR_H
#define DECOMPRESSOR_H

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <queue>
#include "../node.h"

using namespace std;

class Decompressor {
private:
    string compressedFilename;
    string decompressedFilename;
    map<char, int> frequencyTable;
    string compressedData;
    map<char, string> huffmanCodes;

public:
    Decompressor(const string& compressedFile, const string& decompressedFile);
    
    void decompress();


private:
    void readCompressedFile();
    Node* reconstructHuffmanTree();
    void decode(Node* root);
    void writeDecodedFile();
    void readFreqTable();
    void buildHuffmanCodes(Node* node, string code);
};

#endif
