#ifndef DECODER_HPP
#define DECODER_HPP

#include "tree.hpp"
#include "bstream.hpp"

tree* reconstruct_tree(ibstream* bin);
void read_prefix_codes(tree* huffman, ibstream* bin, std::ostream* out);
void decode(std::string filename);

#endif
