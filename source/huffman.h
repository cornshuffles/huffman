#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <string>
#include <vector>
#include <map>

// Huffman Tree node struct
struct MinHeapNode{
    // Element represented by this node
    std::string data;
    // Frequency of the element
    uint freq;
    // Pointer to left and right child of this node
    struct MinHeapNode *left, *right;
};

// Huffman Tree struct
struct MinHeap{
    // Current size of min heap
    uint size;
    // capacity of min heap
    uint capacity;
    // Array of minheap node pointers
    struct MinHeapNode** array;
};

// Prototype functions
std::map<std::string,std::string>* HuffmanCodes(std::string data[], int freq[], int size);
MinHeapNode* buildHuffmanTreeFromCodes(std::vector<std::pair<std::string, std::string>> stringCode);
int isLeaf(struct MinHeapNode* root);

#endif