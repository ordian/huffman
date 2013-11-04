#ifndef HUFFMAN_H_
#define HUFFMAN_H_

#include <fstream>

int huffmanEncodeFile(std::ifstream& in, std::ofstream& out);
int huffmanDecodeFile(std::ifstream& in, std::ofstream& out);

#endif /* HUFFMAN_H_ */
