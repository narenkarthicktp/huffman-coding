#include "encoder.hpp"
#include <iostream>
#include <unordered_map>

std::unordered_map<char, int> get_byte_frequency(std::string source_file)
{
	std::ifstream fin(source_file);
	std::unordered_map<char, int> frequency = std::unordered_map<char, int>();
	char c;

	if (fin)
	{
		while(fin.get(c))
		{
			if(frequency.find(c) == frequency.end())
				frequency[c] = 0;
			frequency[c] += 1;
		}
		fin.close();
	}

	return frequency;
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

void write_size(unsigned long long total_bits, obstream* bout)
{
	if(!bout || !(bout->is_open()))
		return;

	for(int i = 7*8; i >= 0; i -= 8)
		bout->write_byte((char)(total_bits >> i) & 255);
}

void write_header(tree* huffman, unsigned long long total_bits, obstream* bout)
{
	if(!bout || !(bout->is_open()))
		return;

	write_size(total_bits, bout);
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

std::string cached_lookup(tree huffman, char c)
{
	// memoization to avoid traversing the tree again
	static std::unordered_map<char, std::string> lookup = std::unordered_map<char, std::string>();
	if(lookup.find(c) == lookup.end())
		lookup[c] = huffman[c];
	return lookup[c];
}
unsigned long long write_prefix_codes(tree* huffman, std::istream* in, obstream* bout)
{
	unsigned long long total_bits = 0;
	char c;

	if(!in || !bout || !(bout->is_open()))
		return 0;

	while(in->get(c))
	{
		std::string bitstring = cached_lookup(*huffman, c);
		total_bits += bitstring.length();
		for(auto bit : bitstring)
			bout->write_bit(bit == '1');
	}

	return total_bits;
}

void encode(std::string source_file, std::string target_file)
{
	// count characters
	std::unordered_map<char, int> frequency = get_byte_frequency(source_file);

	if(frequency.size() == 0)
	{
		std::cout<<"File empty or enable to open";
		return;
	}

	// grow forest :p
	minheap forest;
	for(auto x: frequency)
		forest.push(new tree(std::string(1, x.first), NULL, NULL, x.second));

	// create huffman tree
	tree* huffman = optimal_merge(forest);

	obstream bout(target_file);
	std::fstream fin(source_file, std::ios_base::in);

	// header contains total_bits and the huffman tree (pass 0 for total_bits as of now)
	write_header(huffman, 0, &bout);
	// encode data using huffman tree
	unsigned long long total_bits = write_prefix_codes(huffman, &fin, &bout);

	// go to start and overwrite the total_bits
	bout.flush();
	bout.seekp(0);
	write_size(total_bits, &bout);

	bout.close();
	fin.close();
}
