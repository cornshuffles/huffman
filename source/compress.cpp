/*
AUTHOR: Vladislav Nikolic
STID: 1079
DATE: November 28, 2020
CLASS: CS315-001 Fall 2020
PROJECT: final project
COMPILATION: g++ *.cpp
SYNOPSIS: Once compiled, execute the program using ./a.out - replace "a.out" with whatever the name of the executable you generated when compiling is.
          By default the program will try to read from "in.txt", it will not work correctly if it cannot find this file.
          You can change the name of the input file by changing the #define inFileName below
COMPUTER: VM allocated by the CS department
ALGORITHM: This program implements the Huffman code generating algorithm
COMPLEXITY: O(nlog(n)) - where n is the number of unique elements
BUGS: None known
REFERENCES: None to report
NOTES: None to report
*/

#include <stdlib.h>
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <math.h> // pow()
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
 * Function:    convertCodesToInt                       *
 * Params:      map<string, string>* stringCode -       *
 *              pointer to a map of elements and codes  *
 *                                                      *
 *              map<string, uint8_t>* string8int -      *
 *              pointer to map to store 8 bit codes in  *
 *                                                      *
 *              map<string, uint16_t>* string16int -    *
 *              pointer to map to store 16 bit codes in *
 *                                                      *
 * Returns:     void                                    *
 *                                                      *
 * Description: populates string8int and string16int    *
 *              maps with integer codes for elements    *
 *              given in stringCode                     *
 * -----------------------------------------------------*/
void convertCodesToInt(map<string, string>* stringCode, map<string, uint8_t>* string8int, map<string, uint16_t>* string16int, map<string,uint24_t>* string24int){
    uint8_t sum8 = 0;
    uint16_t sum16 = 0;
    uint24_t sum24;
    sum24.data = 0;
    char c;
    int temp = string8int->size();
    auto it = string8int->begin();

    // Create a map of chars and codes that can be written to a file
    for(map<string, string>::iterator it = stringCode->begin(); it != stringCode->end(); it++){
        if(it->second.length() == 8){
            for(int i = 0; i < 8; i++){
                c = it->second[i];
                uint8_t temp = atoi(&c);
                sum8 += temp * pow(2, 8 - (1 + i));
            }
            string8int->insert(pair<string, uint8_t>(it->first, sum8));
            sum8 = 0;
        }
        else if(it->second.length() == 16){
            for(int i = 0; i < 16; i++){
                c = it->second[i];
                uint16_t temp = atoi(&c);
                sum16 += temp * pow(2, 16 - (1 + i));
            }
            string16int->insert(pair<string, uint16_t>(it->first, sum16));
            sum16 = 0;
        }
        else if(it->second.length() == 24){
            for(int i = 0; i < 24; i++){
                c = it->second[i];
                uint24_t temp;
                temp.data = atoi(&c);
                sum24.data += temp.data * pow(2, 24 - (1 + i));
            }
            string24int->insert(pair<string, uint24_t>(it->first, sum24));
            sum24.data = 0;
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
void writeCompressedFile(map<string, uint8_t>* string8int, map<string, uint16_t>* string16int, map<string, uint24_t>* string24int){
    // Write number of elements, elements, and codes to the output file
    ofstream OFStream(COMPRESSEDFILENAME, ios::binary);
    // Write number of elements
    OFStream << string8int->size() << " " << string16int->size() << " " << string24int->size();
    //  Write Elements and codes
    for(auto it = string8int->begin(); it != string8int->end(); it++){
        OFStream << (uint8_t)it->first.size() << it->first << it->second;
    }
    for(auto it = string16int->begin(); it != string16int->end(); it++){
        uint16_t temp = it->second;
        uint8_t first, second = 0;
        first = (uint8_t)(temp >> 8);
        second = (uint8_t)temp;
        OFStream << (uint8_t)it->first.size() << it->first << first << second;
    }
    for(auto it = string24int->begin(); it != string24int->end(); it++){
        uint24_t temp = it->second;
        uint8_t first, second, third = 0;
        first = (uint8_t)(temp.data >> 16);
        second = (uint8_t)(temp.data >> 8);
        third = (uint8_t)temp.data;
        OFStream << (uint8_t)it->first.size() << it->first << first << second << third;
    }

    // Vars for writing data to the compressed file
    string gottenWord = "";
    char gottenChar;
    map<string, uint8_t>::iterator it;
    map<string, uint16_t>::iterator it16;
    map<string, uint24_t>::iterator it24;

    // Write compressed data to file
    ifstream IFStream;
    IFStream.open(INFILENAME);
    if(IFStream.is_open()){
        while(IFStream.get(gottenChar)){
            if((gottenChar > 96 && gottenChar < 123) || (gottenChar > 64 && gottenChar < 91)){
                gottenWord += gottenChar;
            }
            else{
                if(gottenWord != ""){
                    it = string8int->find(gottenWord); // Search 8 bit codes
                    if(it != string8int->end()){
                        OFStream << it->second;
                    }
                    else{
                        it16 = string16int->find(gottenWord); // Search 16 bit codes
                        if(it16 != string16int->end()){
                            uint16_t temp = it16->second;
                            uint8_t first, second = 0;
                            first = (uint8_t)(temp >> 8);
                            second = (uint8_t)temp;
                            OFStream << first << second;
                        }
                        else{
                            it24 = string24int->find(gottenWord); // Search 24 bit codes
                            if(it24 != string24int->end()){
                                uint24_t temp = it24->second;
                                uint8_t first, second, third = 0;
                                first = (uint8_t)(temp.data >> 16);
                                second = (uint8_t)(temp.data >> 8);
                                third = (uint8_t)temp.data;
                                OFStream << first << second << third;
                            }
                        }
                    }
                }
                gottenWord = gottenChar;
                it = string8int->find(gottenWord); // Search 8 bit codes
                if(it != string8int->end()){
                    OFStream << it->second;
                }
                else{
                    it16 = string16int->find(gottenWord); // Search 16 bit codes
                    if(it16 != string16int->end()){
                        uint16_t temp = it16->second;
                        uint8_t first, second = 0;
                        first = (uint8_t)(temp >> 8);
                        second = (uint8_t)temp;
                        OFStream << first << second;
                    }
                    else{
                        it24 = string24int->find(gottenWord); // Search 24 bit codes
                        if(it24 != string24int->end()){
                            uint24_t temp = it24->second;
                            uint8_t first, second, third = 0;
                            first = (uint8_t)(temp.data >> 16);
                            second = (uint8_t)(temp.data >> 8);
                            third = (uint8_t)temp.data;
                            OFStream << first << second << third;
                        }
                    }
                }
                gottenWord = "";

            }
        }
    }
    else{
        cout << "Couldn't open in.txt" << endl;
    }
    // Insert the last word
    if(gottenWord != ""){
        it = string8int->find(gottenWord); // Search 8 bit codes
        if(it != string8int->end()){
            OFStream << it->second;
        }
        else{
            it16 = string16int->find(gottenWord); // Search 16 bit codes
            if(it16 != string16int->end()){
                uint16_t temp = it16->second;
                uint8_t first, second = 0;
                first = (uint8_t)(temp >> 8);
                second = (uint8_t)temp;
                OFStream << first << second;
            }
            else{
                it24 = string24int->find(gottenWord); // Search 24 bit codes
                if(it24 != string24int->end()){
                    uint24_t temp = it24->second;
                    uint8_t first, second, third = 0;
                    first = (uint8_t)(temp.data >> 16);
                    second = (uint8_t)(temp.data >> 8);
                    third = (uint8_t)temp.data;
                    OFStream << first << second << third;
                }
            }
        } 
    }
    IFStream.close();
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
    // Read codes into maps
    ifstream myifStream(COMPRESSEDFILENAME, ios::binary);
    string gottenElement = "";
    char gottenChar;
    uint8_t gottenCode = 0;
    uint16_t gottenCode16 = 0;
    uint24_t gottenCode24;
    gottenCode24.data = 0;
    map<string, uint8_t> readin;
    map<string, uint16_t> readin16;
    map<string, uint24_t> readin24;
    int size8, size16, size24 = 0;
    
    // Read number of elements
    myifStream >> size8;
    myifStream >> size16;
    myifStream >> size24;
    
    // Read all the 8 bit elements
    for(int i = 0; i < size8; i++){
        // Get element length
        myifStream.get(gottenChar);
        uint8_t elementLength = (uint8_t)gottenChar;
        // Get element
        for(int j = 0; j < elementLength; j++){
            myifStream.get(gottenChar);
            gottenElement += gottenChar;
        }
        // Get code
        myifStream.get(gottenChar);
        gottenCode = (uint8_t)gottenChar;
        // Insert the element and code into the map
        readin.insert(pair<string, uint8_t>(gottenElement, gottenCode));
        gottenElement = "";
    }
    // Read all the 16 bit elements
    for(int i = 0; i < size16; i++){
        // Get element length
        myifStream.get(gottenChar);
        uint8_t elementLength = (uint8_t)gottenChar;
        // Get element
        for(int j = 0; j < elementLength; j++){
            myifStream.get(gottenChar);
            gottenElement += gottenChar;
        }
        // Get code
        myifStream.get(gottenChar);
        gottenCode16 = ((uint8_t)gottenChar) << 8;
        myifStream.get(gottenChar);
        gottenCode16 |= (uint8_t)gottenChar;
        // Insert the element and code into the map
        readin16.insert(pair<string, uint16_t>(gottenElement, gottenCode16));
        gottenElement = "";
        gottenCode16 = 0;
    }
    // Read all the 24 bit elements
    for(int i = 0; i < size24; i++){
        // Get element length
        myifStream.get(gottenChar);
        uint8_t elementLength = (uint8_t)gottenChar;
        // Get element
        for(int j = 0; j < elementLength; j++){
            myifStream.get(gottenChar);
            gottenElement += gottenChar;
        }
        // Get code
        myifStream.get(gottenChar);
        gottenCode24.data = ((uint8_t)gottenChar) << 16;
        myifStream.get(gottenChar);
        gottenCode24.data |= ((uint8_t)gottenChar) << 8;
        myifStream.get(gottenChar);
        gottenCode24.data |= (uint8_t)gottenChar;
        // Insert the element and code into the map
        readin24.insert(pair<string, uint24_t>(gottenElement, gottenCode24));
        gottenElement = "";
        gottenCode24.data = 0;
    }

    // Recreate the original file
    ofstream decompressed(DECOMPRESSEDFILENAME);
    bool found;
    uint16_t temp16;

    while(myifStream.get(gottenChar)){
        uint8_t temp = (uint8_t)gottenChar;
        // Iterate through the map to find the key we just read
        for(auto it = readin.begin(); it != readin.end(); it++){
            if(it->second == temp){
                decompressed << it->first;
                found = true;
                break;
            }
            found = false;
        }
        if(found == false){
            temp16 = (((uint16_t)gottenChar) << 8);
            myifStream.get(gottenChar);
            temp16 |= (uint8_t)gottenChar;
            for(auto it = readin16.begin(); it != readin16.end(); it++){
                if(it->second == temp16){
                    decompressed << it->first;
                    found = true;
                    break;
                }
                found = false;
            }
        }
        if(found == false){
            uint24_t temp24;
            temp24.data = temp16 << 8;
            myifStream.get(gottenChar);
            temp24.data |= (uint8_t)gottenChar;
            for(auto it = readin24.begin(); it != readin24.end(); it++){
                if(it->second.data == temp24.data){
                    decompressed << it->first;
                    found = true;
                    break;
                }
            }
        }
    }
    myifStream.close();
    decompressed.close();
}


// The main will read in.txt, generate the compressed version compressed.txt, then read compressed.txt and generate the decompressed version decompressed.txt
int main(){ 

    // Get elements and frequencies
    map<string, int>* stringFreq = readInputFile();

    // Get Huffman Codes
    map<string,string>* stringCode = getHuffmanCodes(stringFreq);

    // SANITY
    int longest = 0;
    // Print Codes
    for(auto it = stringCode->begin(); it != stringCode->end(); it++){
        if(it->second.length() > longest){
            longest = it->second.length();
        }
    }
    cout << "Longest Code: " << longest << endl; // Only codes up to 24 bits in length are supported

    // Vars to store the codes that can be sent to file
    map<string, uint8_t>* string8int = new map<string, uint8_t>();
    map<string, uint16_t>* string16int = new map<string, uint16_t>();
    map<string, uint24_t>* string24int = new map<string, uint24_t>();

    // Convert Huffman Codes from strings to ints
    convertCodesToInt(stringCode, string8int, string16int, string24int);

    // Write compressed file
    writeCompressedFile(string8int, string16int, string24int);

    // Decompress
    decompress();

    return 0;
}
