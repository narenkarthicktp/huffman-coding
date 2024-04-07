#include "encoder.hpp"

unsigned int* get_byte_frequency(std::string source_file)
{
	std::ifstream fin(source_file);
	unsigned int* frequency_array = (unsigned int*)calloc(256, sizeof(unsigned int));
	char c;

	if (!fin) {
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
	if(!bout)
		return;

	std::string meta = huffman->to_string();
	for(auto x : meta)
	{
		if(x != '0' && x != '1')
			bout->write_byte(x);
		else
			bout->write_bit(x == '1');
	}
	bout->write_bit(0);
}

unsigned int write_prefix_codes(tree* huffman, std::string source_file, obstream* bout)
{
	std::ifstream fin(source_file);
	unsigned int total_bits = 0;
	char c;

	if(!fin or !bout)
		return 0;

	while(fin.get(c))
	{
		std::string bitstring = (*huffman)[c];
		total_bits += bitstring.length();
		for(auto bit : bitstring)
			bout->write_bit(bit == '1');
	}
	fin.close();

	return total_bits;
}

std::string encode(std::string source_file, std::string target_file)
{
	// COUNT CHARACTERS
	unsigned int* frequency_array = get_byte_frequency(source_file);

	// GROW FOREST :P
	minheap forest;
	for(int i = 0; i < 256; i++)
	{
		if(frequency_array[i])
			forest.push(new tree(std::string(1, i), NULL, NULL, frequency_array[i]));
	}

	// CREATE HUFFMAN TREE
	tree* huffman = optimal_merge(forest);
	// tree::inorder(huffman);
	// std::cout<<huffman->to_string()<<std::endl;

	obstream bout(target_file);
	write_header(huffman, &bout);
	write_prefix_codes(huffman, source_file, &bout);
	bout.close();

	return target_file;
}
