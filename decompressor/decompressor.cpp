#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "decompressor.h"
#include <sstream>

using namespace std;

Decompressor::Decompressor(const string& compressedFile, const string& decompressedFile)
    : compressedFilename(compressedFile), decompressedFilename(decompressedFile) {}

void Decompressor::decompress() {
    readFreqTable();
    readCompressedFile();
    Node* root = reconstructHuffmanTree();
    buildHuffmanCodes(root, "");  // Provide an empty initial code
    decode(root);
    writeDecodedFile();
}

void Decompressor::readFreqTable() {
    ifstream inputFile("freq.bin", ios::binary);
    if (!inputFile) {
        cout << "Error: Unable to open the compressed freq table file." << endl;
        return;
    }

    int tableSize;
    inputFile.read(reinterpret_cast<char*>(&tableSize), sizeof(int));

    for (int i = 0; i < tableSize; ++i) {
        char character;
        int frequency;
        inputFile.read(reinterpret_cast<char*>(&character), sizeof(char));
        inputFile.read(reinterpret_cast<char*>(&frequency), sizeof(int));
        this->frequencyTable[character] = frequency;
        cout << character << " " << frequency << endl;
    }
    inputFile.close();
}

void Decompressor::readCompressedFile() {
    ifstream inputFile(compressedFilename, ios::binary);
    if (!inputFile) {
        cout << "Error: Unable to open the compressed file." << endl;
        return;
    }

    // Determine the size of the file
    inputFile.seekg(0, ios::end);
    int fileSize = inputFile.tellg();
    inputFile.seekg(0, ios::beg);

    cout << "File size: " << fileSize << endl;

    // Read the compressed data into the compressedData string
    compressedData.resize(fileSize);
    inputFile.read(&compressedData[0], fileSize);

    inputFile.close();
}


Node* Decompressor::reconstructHuffmanTree() {
    auto compare = [](const Node* left, const Node* right) {
        return left->frequency > right->frequency;
    };
    priority_queue<Node*, vector<Node*>, decltype(compare)> pq(compare);

    // Build the priority queue with leaf nodes
    for (const auto& entry : this->frequencyTable) {
        char character = entry.first;
        int frequency = entry.second;
        pq.push(new Node(character, frequency));
    }

    // Reconstruct the Huffman tree
    while (pq.size() > 1) {
        Node* leftNode = pq.top();
        pq.pop();
        Node* rightNode = pq.top();
        pq.pop();

        Node* internalNode = new Node('\0', leftNode->frequency + rightNode->frequency);
        internalNode->left = leftNode;
        internalNode->right = rightNode;

        pq.push(internalNode);
    }

    return pq.top();
}

void Decompressor::buildHuffmanCodes(Node* node, string code) {
    if (node == nullptr) {
        return;
    }

    // Leaf node: Assign the Huffman code
    if (node->left == nullptr && node->right == nullptr) {
        huffmanCodes[node->character] = code;
    }

    // Traverse left branch (append '0' to the code) and recursively assign codes
    buildHuffmanCodes(node->left, code + '0');

    // Traverse right branch (append '1' to the code) and recursively assign codes
    buildHuffmanCodes(node->right, code + '1');
}

void Decompressor::decode(Node* root) {
    ofstream outputFile(decompressedFilename, ios::binary);
    if (!outputFile) {
        cout << "Error: Unable to open the output file." << endl;
        return;
    }

    Node* currentNode = root;
    int bitCount = 0;
    char byte = 0;

    for (size_t i = 0; i < compressedData.size(); ++i) {
        byte = compressedData[i];

        for (int j = 7; j >= 0; --j) {
            bool isBitSet = (byte >> j) & 1;

            if (isBitSet)
                currentNode = currentNode->right;
            else
                currentNode = currentNode->left;

            if (currentNode->left == nullptr && currentNode->right == nullptr) {
                outputFile.write(&currentNode->character, sizeof(char));
                currentNode = root;
            }
        }
    }

    outputFile.close();
}


void Decompressor::writeDecodedFile() {
    ifstream inputFile(decompressedFilename, ios::binary);
    if (!inputFile) {
        cout << "Error: Unable to open the decoded file." << endl;
        return;
    }

    stringstream buffer;
    buffer << inputFile.rdbuf();
    string content = buffer.str();
    cout << "Content: " << content << endl;

    inputFile.close();
}
