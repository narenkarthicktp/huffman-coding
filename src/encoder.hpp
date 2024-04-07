#ifndef ENCODER_HPP
#define ENCODER_HPP

#include <queue>
#include "tree.hpp"
#include "bstream.hpp"

typedef std::priority_queue<tree*, std::vector<tree*>, tree> minheap;

unsigned int* get_byte_frequency(std::string source_file);
tree* optimal_merge(minheap forest);
void write_header(tree* huffman, obstream* bout);
unsigned int write_prefix_codes(tree* huffman, std::istream* in, obstream* bout);
void encode(std::string source_file, std::string target_file);

#endif
