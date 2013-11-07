#ifndef HUFFMAN_H_
#define HUFFMAN_H_

#include "node.h"
#include <fstream>

int huffmanEncodeFile(std::ifstream& in, std::ofstream& out, Size_t size);
int huffmanDecodeFile(std::ifstream& in, std::ofstream& out);

#endif /* HUFFMAN_H_ */
