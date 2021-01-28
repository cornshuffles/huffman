/*
AUTHOR: Vladislav Nikolic
DATE: November 28, 2020
COMPILATION: g++ *.cpp
SYNOPSIS: Once compiled, execute the program using ./a.out - replace "a.out" with whatever the name of the executable you generated when compiling is
ALGORITHM: This program implements the Huffman code generating algorithm
COMPLEXITY: O(nlog(n)) - where n is the number of unique elements
BUGS: None known
REFERENCES: https://www.geeksforgeeks.org/huffman-coding-greedy-algo-3/ - This code is a modified version of the code available here
NOTES: The input, compressed, and decompressed file names are specified by macros in compress.cpp
*/

#include "huffman.h"
#include "compress.h"

using namespace std;

// The main will read in.txt, generate the compressed version compressed.txt, then read compressed.txt and generate the decompressed version decompressed.txt
int main(){ 
    // Get elements and frequencies
    map<string, int>* stringFreq = readInputFile();

    // Get Huffman Codes
    map<string,string>* stringCode = getHuffmanCodes(stringFreq);

    // Write compressed file
    writeCompressedFile(stringCode);

    // Decompress
    decompress();

    return 0;
}