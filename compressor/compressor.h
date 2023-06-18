#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <string>
#include <map>
#include <queue>
#include <iostream>
#include <iomanip>

using namespace std;

struct Node {
    char character;
    int frequency;
    Node *left, *right;

    Node(char ch, int freq) : character(ch), frequency(freq), left(nullptr), right(nullptr) {}
};

struct Compare {
    bool operator()(Node *a, Node *b) {
        return a->frequency > b->frequency;
    }
};

class Compressor {
    public:
        string text;
        map<char, int> frequencyTable;
        map<char, string> huffmanCodes;
        
        Compressor(string text);

        string compress();

        string decompress();

        void reportCompression(long originalFileSize, streampos compressedFileSize);
    
    private:
        void buildFrequencyTable();

        Node *buildHuffmanTree();

        void assignHuffmanCodes(Node* node, string code);

        string encode();

};


#endif