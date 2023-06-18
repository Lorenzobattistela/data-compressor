#include "compressor.h"

#include <iostream>
#include <queue>
#include <map>
#include <iomanip>

using namespace std;

Compressor::Compressor(string text) {
    this->text = text;
}

string Compressor::compress() {
    cout << "Compressing..." << endl;
    this->buildFrequencyTable();
    Node *root = this->buildHuffmanTree();
    this->assignHuffmanCodes(root, "");
    return this->encode();
}

string Compressor::decompress() {
    return "Decompressing...";
}

void Compressor::buildFrequencyTable() {
    int charCount = 0;
    for(int i = 0; i < this->text.length(); i++) {
        charCount++;
        if(this->frequencyTable.find(this->text[i]) == this->frequencyTable.end()) {
            this->frequencyTable.insert(pair<char, int>(this->text[i], 1));
        } else {
            this->frequencyTable[this->text[i]]++;
        }
    }

    std::map<char, int> updatedFrequencyTable = this->frequencyTable;

    for (auto& x : updatedFrequencyTable) {
        x.second = x.second / charCount;
    }

    this->frequencyTable = updatedFrequencyTable;
}

Node* Compressor::buildHuffmanTree() {
    priority_queue<Node*, vector<Node*>, Compare> pq;

    for (const auto& entry : this->frequencyTable) {
        char character = entry.first;
        int frequency = entry.second;
        pq.push(new Node(character, frequency));
    }

    while(pq.size() > 1) {
        Node *leftNode = pq.top();
        pq.pop();
        Node *rightNode = pq.top();
        pq.pop();

        Node *internalNode = new Node('\0', leftNode->frequency + rightNode->frequency);

        internalNode->left = leftNode;
        internalNode->right = rightNode;

        pq.push(internalNode);
    }
    return pq.top();
}

void Compressor::assignHuffmanCodes(Node* node, string code) {
    if(node == nullptr) {
        return;
    }

    // Leaf node: Assign the Huffman code
    if (node->left == nullptr && node->right == nullptr) {
        this->huffmanCodes[node->character] = code;
    }

    // Traverse left branch (append '0' to the code) and recursively assign codes
    assignHuffmanCodes(node->left, code + '0');

    // Traverse right branch (append '1' to the code) and recursively assign codes
    assignHuffmanCodes(node->right, code + '1');
}

string Compressor::encode() {
    string encodedText = "";
    for(int i = 0; i < this->text.length(); i++) {
        encodedText += this->huffmanCodes[this->text[i]];
    }

    return encodedText;
}

void Compressor::reportCompression(long originalFileSize, streampos compressedFileSize) {
    cout << "Original file size: " << originalFileSize << " bytes" << endl;
    cout << "Compressed file size: " << compressedFileSize << " bytes" << endl;
    cout << "Compression ratio: " << (float)originalFileSize / (float)compressedFileSize << endl;

    float val = ((float)compressedFileSize * 100) / (float)originalFileSize;

    cout << "File shortened " << fixed << setprecision(2) << val << "%" << endl;
}