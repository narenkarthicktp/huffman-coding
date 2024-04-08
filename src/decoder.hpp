#ifndef DECODER_HPP
#define DECODER_HPP

#include "tree.hpp"
#include "bstream.hpp"

tree* reconstruct_tree(ibstream* bin);
void read_prefix_codes(tree* huffman, ibstream* bin, std::ostream* out, unsigned long long total_bits);
unsigned long long read_size(ibstream* bin);
void decode(std::string source_file, std::string target_file);

#endif
