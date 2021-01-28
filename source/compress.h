#ifndef COMPRESS_H
#define COMPRESS_H

#include <map>
#include <string>

// Prototype functions
std::map<std::string,int>* readInputFile();
std::map<std::string, std::string>* getHuffmanCodes(std::map<std::string, int>* stringFreq);
void decompress();
void writeCompressedFile(std::map<std::string, std::string>* stringCode);
#endif