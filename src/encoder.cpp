#include "encoder.hpp"
#include <iostream>

unsigned int* get_byte_frequency(std::string source_file)
{
	std::ifstream fin(source_file);
	unsigned int* frequency_array = (unsigned int*)calloc(256, sizeof(unsigned int));
	char c;

	if (!fin)
	{
		std::cout<<"Unable to open file";
		return NULL;
	}
	while(fin.get(c))
		frequency_array[c] += 1;
	fin.close();

	return frequency_array;
}

tree* optimal_merge(minheap forest)
{
	while(forest.size() > 1)
	{
		tree* left = forest.top(); forest.pop();
		tree* right = forest.top(); forest.pop();
		forest.push(new tree(left->data+right->data, left, right, left->frequency+right->frequency));
	}
	return forest.top();
}

void write_header(tree* huffman, obstream* bout)
{
	if(!bout || !(bout->is_open()))
		return;

	std::string meta = huffman->to_string();

	// 0s and 1s in the source file will conflict with the delimiters used in tree::to_string()
	bool write_byte_flag = false;
	for(auto x : meta)
	{
		if(write_byte_flag)
			bout->write_byte(x);
		else
			bout->write_bit(x == '1');
		// 1 is always followed by a character (that character could also be 1)
		write_byte_flag = write_byte_flag ? false : (x == '1');
	}
	bout->write_bit(0);
}

unsigned int write_prefix_codes(tree* huffman, std::istream* in, obstream* bout)
{
	unsigned int total_bits = 0;
	char c;

	if(!in || !bout || !(bout->is_open()))
		return 0;

	while(in->get(c))
	{
		std::string bitstring = (*huffman)[c];
		total_bits += bitstring.length();
		for(auto bit : bitstring)
			bout->write_bit(bit == '1');
	}

	return total_bits;
}

void encode(std::string source_file, std::string target_file)
{
	// count characters
	unsigned int* frequency_array = get_byte_frequency(source_file);

	// grow forest :p
	minheap forest;
	for(int i = 0; i < 256; i++)
	{
		if(frequency_array[i])
			forest.push(new tree(std::string(1, i), NULL, NULL, frequency_array[i]));
	}

	// create huffman tree
	tree* huffman = optimal_merge(forest);

	obstream bout(target_file);
	std::fstream fin(source_file, std::ios_base::in);

	// header contains the huffman tree
	write_header(huffman, &bout);
	// encode data using huffman tree
	write_prefix_codes(huffman, &fin, &bout);

	fin.close();
	bout.close();
}
