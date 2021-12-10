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

#include <fstream>
#include <iostream>
#include "huffman.h"
#include "compress.h"

#define INFILENAME              "in.txt"
#define COMPRESSEDFILENAME      "compressed.txt"
#define DECOMPRESSEDFILENAME    "decompressed.txt"

using namespace std;

/*------------------------------------------------------*
 * Function:    readInputFile                           *
 * Params:      none                                    *
 *                                                      *
 * Returns:     map<string, int>* - Pointer to created  *
 *                                  map containing the  *
 *              elements extracted from the in file and *
 *              their frequencies                       *
 *                                                      *
 * Description: returns a pointer to a newly allocated  *
 *              map of elements and their frequencies   *
 * -----------------------------------------------------*/
map<string,int>* readInputFile(){
    ifstream IFStream (INFILENAME);
    char gottenChar;
    string gottenWord;

    // map to hold elements and frequencies
    map<string, int>* stringFreq = new map<string, int>();
    // Var to check value returned by map.insert
    pair<map<string,int>::iterator, bool> result;

    // Iterate over the input file and put all words and characters into the map.
    // If the file is open
    if(IFStream.is_open()){
        // While we haven't reached the end of the file
        while(IFStream.get(gottenChar)){
            // If it was a letter add it to the word
            if((gottenChar > 96 && gottenChar < 123) || (gottenChar > 64 && gottenChar < 91)){
                gottenWord += gottenChar;
            }
            else{
                if(gottenWord != ""){
                    result = stringFreq->insert(pair<string, int>(gottenWord,1));
                    // Element has already been encountered, increase its frequency
                    if(result.second == false){
                        result.first->second++;
                    }
                }
                gottenWord = gottenChar;
                result = stringFreq->insert(pair<string, int>(gottenWord,1));
                // Element has already been encountered, increase its frequency
                if(result.second == false){
                    result.first->second++;
                }
                gottenWord = "";

            }
        }
        // Insert the last word
        if(gottenWord != ""){
            result = stringFreq->insert(pair<string, int>(gottenWord,1));
            // Element has already been encountered, increase its frequency
            if(result.second == false){
                result.first->second++;
            }
        }
    }
    else{
        cout << "Couldn't open in.txt" << endl;
        return NULL;
    }
    IFStream.close();

    return stringFreq;
}

/*------------------------------------------------------*
 * Function:    getHuffmanCodes                         *
 * Params:      map<string, int>* stringFreq - pointer  *
 *                                             to map   *
 *              of elements and frequencies to generate *
 *              codes for                               *
 *                                                      *
 * Returns:     map<string, string>* - Pointer to map   *
 *                                     containing the   *
 *              elements and their codes as strings     *
 *                                                      *
 * Description: returns a pointer to a newly allocated  *
 *              map of elements and their codes         *
 * -----------------------------------------------------*/
map<string, string>* getHuffmanCodes(map<string, int>* stringFreq){
    // Vars to store elements and frequencies in
    // These are what the Huffman algorithm code takes
    int size = stringFreq->size();
    string arr[size];
    int freq[size];
    int i = 0;
    // Populate arrays with elements and frequencies
    for(map<string, int>::iterator it = stringFreq->begin(); it != stringFreq->end(); it++){
        arr[i] = it->first;
        freq[i] = it->second;
        i++;
    }
    // Get Huffman Codes
    return HuffmanCodes(arr, freq, size);
}

/*------------------------------------------------------*
 * Function:    writeCode                               *
 * Params:      string code - code to write to OFStream *
 *              in string form                          *
 *                                                      *
 *              uint8_t *writeBuffer - the buffer to    *
 *              write codes into before writing them to *
 *              the OFStream                            *
 *                                                      *
 *              uint8_t *bufferIndex - position in the  *
 *              buffer codes will be written at         *
 *                                                      *
 *              ofstream *OFStream - the stream to      *
 *              write codes to                          *
 *                                                      *
 * Returns:     void                                    *
 *                                                      *
 * Description: writes the compressed file - called     *
 *              compressed.txt by default               *
 *              To flush the buffer call with code:     *
 *              "flush                  "               *
 * -----------------------------------------------------*/
void writeCode(string code, uint8_t *writeBuffer, uint8_t *bufferIndex, ofstream *OFStream){
    uint8_t codeIndex = 0;
    char charToPut = 0;

    if(code == "flush" && *bufferIndex != 0){
        charToPut = *writeBuffer;
        OFStream->put(charToPut);
        *bufferIndex = 0;
        *writeBuffer = 0;
    }
    else{
        while(codeIndex < code.length()){
            if(code[codeIndex] == '1'){
                *writeBuffer |= (uint8_t)(1 << (7 - *bufferIndex));
            }
            (*bufferIndex)++;
            if(codeIndex < code.length()){
                codeIndex++;
            }
            else{
                break;
            }
            if(*bufferIndex == 8){
                charToPut = *writeBuffer;
                OFStream->put(charToPut);
                *bufferIndex = 0;
                *writeBuffer = 0;
            }
        }
    }
}

/*------------------------------------------------------*
 * Function:    writeCompressedFile                     *
 * Params:      map<string, uint8_t>* string8int -      *
 *              pointer to map to store 8 bit codes in  *
 *                                                      *
 *              map<string, uint16_t>* string16int -    *
 *              pointer to map to store 16 bit codes in *
 *                                                      *
 * Returns:     void                                    *
 *                                                      *
 * Description: writes the compressed file - called     *
 *              compressed.txt by default               *
 * -----------------------------------------------------*/
void writeCompressedFile(map<string, string>* stringCode){
    ofstream OFStream(COMPRESSEDFILENAME, ios::binary);

    // Write number of elements
    OFStream << stringCode->size();

    //  Write Element lengths, Elements, and code lengths to the compressed file
    for(auto elementIterator = stringCode->begin(); elementIterator != stringCode->end(); elementIterator++){
        char charToPut;
        charToPut = (uint8_t)elementIterator->first.size();
        OFStream.put(charToPut);
        OFStream << elementIterator->first;
        charToPut = (uint8_t)elementIterator->second.length();
        OFStream.put(charToPut);
    }

    // Vars for writing codes to the compressed file
    uint8_t writeBuffer = 0;
    uint8_t bufferIndex = 0;
    string flush = "flush";

    // Write codes
    for(auto elementIterator = stringCode->begin(); elementIterator != stringCode->end(); elementIterator++){
        writeCode(elementIterator->second, &writeBuffer, &bufferIndex, &OFStream);
    }
    // Flush the write buffer
    writeCode(flush, &writeBuffer, &bufferIndex, &OFStream);

    // Vars for writing data to the compressed file
    string gottenWord = "";
    char gottenChar;
    ifstream IFStream;
    auto elementIterator = stringCode->begin();
    unsigned long long int numberOfCompressedBits = 0;

    // Add up how many bits of compressed data will be written to the compressed file
    IFStream.open(INFILENAME);
    if(IFStream.is_open()){
        while(IFStream.get(gottenChar)){
            if((gottenChar > 96 && gottenChar < 123) || (gottenChar > 64 && gottenChar < 91)){
                gottenWord += gottenChar;
            }
            else{
                if(gottenWord != ""){
                    elementIterator = stringCode->find(gottenWord); // Search codes for word element
                    numberOfCompressedBits += elementIterator->second.length();
                }
                gottenWord = gottenChar;
                elementIterator = stringCode->find(gottenWord); // Search codes for single character element
                numberOfCompressedBits += elementIterator->second.length();
                gottenWord = "";
            }
        }
    }
    else{
        cout << "Couldn't open in.txt" << endl;
    }
    // Count the last word
    if(gottenWord != ""){
        elementIterator = stringCode->find(gottenWord); // Search all codes for final element
        numberOfCompressedBits += elementIterator->second.length();
    }
    gottenWord = "";
    gottenChar = 0;
    IFStream.close();

    // Write the number of compressed data bits to the compressed file one byte at a time
    char charToPut;
    uint8_t numberOfCompressedBits1 = 0;
    uint8_t numberOfCompressedBits2 = 0;
    uint8_t numberOfCompressedBits3 = 0;
    uint8_t numberOfCompressedBits4 = 0;
    numberOfCompressedBits1 = (numberOfCompressedBits >> 24);
    numberOfCompressedBits2 = (numberOfCompressedBits >> 16);
    numberOfCompressedBits3 = (numberOfCompressedBits >> 8);
    numberOfCompressedBits4 = numberOfCompressedBits;
    charToPut = numberOfCompressedBits1;
    OFStream.put(charToPut);
    charToPut = numberOfCompressedBits2;
    OFStream.put(charToPut);
    charToPut = numberOfCompressedBits3;
    OFStream.put(charToPut);
    charToPut = numberOfCompressedBits4;
    OFStream.put(charToPut);

    // Write compressed data to the file
    IFStream.open(INFILENAME);
    if(IFStream.is_open()){
        while(IFStream.get(gottenChar)){
            if((gottenChar > 96 && gottenChar < 123) || (gottenChar > 64 && gottenChar < 91)){
                gottenWord += gottenChar;
            }
            else{
                if(gottenWord != ""){
                    elementIterator = stringCode->find(gottenWord); // Search codes for word element
                    writeCode(elementIterator->second, &writeBuffer, &bufferIndex, &OFStream);
                }
                gottenWord = gottenChar;
                elementIterator = stringCode->find(gottenWord); // Search codes for single character element
                writeCode(elementIterator->second, &writeBuffer, &bufferIndex, &OFStream);
                gottenWord = "";
            }
        }
    }
    else{
        cout << "Couldn't open in.txt" << endl;
    }
    // Insert the last word
    if(gottenWord != ""){
        elementIterator = stringCode->find(gottenWord); // Search all codes for final element
        writeCode(elementIterator->second, &writeBuffer, &bufferIndex, &OFStream);
    }
    IFStream.close();
    // Flush the write buffer
    writeCode(flush, &writeBuffer, &bufferIndex, &OFStream);
    OFStream.close();
}

/*------------------------------------------------------*
 * Function:    decompress                              *
 * Params:      none                                    *
 *                                                      *
 * Returns:     void                                    *
 *                                                      *
 * Description: reads the compressed file and writes a  *
 *              decompressed version - called           *
 *              decompressed.txt by default             *
 * -----------------------------------------------------*/
void decompress(){
    // Read elements and codes into map
    ifstream IFStream(COMPRESSEDFILENAME, ios::binary);

    vector<pair<string, uint8_t>> stringCodeLength;
    vector<pair<string, string>> stringCode;

    size_t numElements = 0;
    
    // Read number of elements
    IFStream >> numElements;

    char gottenChar;
    // Read all the elements and code lengths
    for(int i = 0; i < numElements; i++){
        uint8_t stringLen = 0;
        IFStream.get(gottenChar);
        stringLen = (uint8_t)gottenChar;

        // Read string
        string gottenWord = "";
        for(int i = 0; i < stringLen; i++){
            IFStream.get(gottenChar);
            gottenWord += gottenChar;
        }

        // Read code length
        uint8_t codeLength = 0;
        IFStream.get(gottenChar);
        codeLength = (uint8_t)gottenChar;

        // Add element and code length to the vector
        stringCodeLength.push_back(pair<string, uint8_t>(gottenWord, codeLength));
    }

    // Read the codes
    uint8_t readBuffer = 0;
    uint8_t bufferIndex = 0;
    string code = "";
    IFStream.get(gottenChar);
    readBuffer = (uint8_t)gottenChar;
    for(auto elementIterator = stringCodeLength.begin(); elementIterator != stringCodeLength.end(); elementIterator++){
        for(int codeIndex = 0; codeIndex < elementIterator->second; codeIndex++){
            if(bufferIndex == 8){
                IFStream.get(gottenChar);
                readBuffer = (uint8_t)gottenChar;
                bufferIndex = 0;
            }
            if((readBuffer & (uint8_t)(1 << (7 - bufferIndex))) == (uint8_t)(1 << (7 - bufferIndex))){
                code += '1';
            }
            else{
                code += '0';
            }
            bufferIndex++;
        }
        stringCode.push_back(pair<string, string>(elementIterator->first, code));
        code = "";
    }

    // Recreate the original file
    ofstream decompressed(DECOMPRESSEDFILENAME);

    MinHeapNode *huffmanTree = buildHuffmanTreeFromCodes(stringCode);
    MinHeapNode *head = huffmanTree;

    // Get the number of compressed bits
    unsigned long long int numberOfCompressedBits = 0;
    unsigned long long int bitsRead = 0;

    uint8_t numberOfCompressedBits1 = 0;
    uint8_t numberOfCompressedBits2 = 0;
    uint8_t numberOfCompressedBits3 = 0;
    uint8_t numberOfCompressedBits4 = 0;

    IFStream.get(gottenChar);
    numberOfCompressedBits1 = (uint8_t)gottenChar;
    IFStream.get(gottenChar);
    numberOfCompressedBits2 = (uint8_t)gottenChar;
    IFStream.get(gottenChar);
    numberOfCompressedBits3 = (uint8_t)gottenChar;
    IFStream.get(gottenChar);
    numberOfCompressedBits4 = (uint8_t)gottenChar;

    numberOfCompressedBits |= numberOfCompressedBits4;
    numberOfCompressedBits |= (numberOfCompressedBits3 << 8);
    numberOfCompressedBits |= (numberOfCompressedBits2 << 16);
    numberOfCompressedBits |= (numberOfCompressedBits1 << 24);

    while(!IFStream.eof()){
        IFStream.get(gottenChar);
        readBuffer = (uint8_t)gottenChar;
        for(bufferIndex = 0; bufferIndex < 8; bufferIndex++){
            if((readBuffer & (uint8_t)(1 << (7 - bufferIndex))) == (uint8_t)(1 << (7 - bufferIndex))){
                head = head->right;
            }
            else{
                head = head->left;
            }
            if(isLeaf(head)){
                decompressed << head->data;
                head = huffmanTree;
            }
            bitsRead++;
            if((int)bitsRead >= (int)numberOfCompressedBits){
                break;
            }
        }
        if((int)bitsRead >= (int)numberOfCompressedBits){
            break;
        }
    }

    IFStream.close();
    decompressed.close();
}
