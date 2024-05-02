#ifndef ENCODER_HPP
#define ENCODER_HPP

#include <queue>
#include <unordered_map>
#include "tree.hpp"
#include "bstream.hpp"

typedef std::priority_queue<tree*, std::vector<tree*>, tree> minheap;

std::unordered_map<char, int> get_byte_frequency(std::string source_file);
tree* optimal_merge(minheap forest);
void write_size(unsigned long long total_bits, obstream* bout);
void write_header(tree* huffman, obstream* bout);
unsigned long long write_prefix_codes(tree* huffman, std::istream* in, obstream* bout);
std::string cached_lookup(tree huffman, char c);
void encode(std::string source_file, std::string target_file);

#endif
