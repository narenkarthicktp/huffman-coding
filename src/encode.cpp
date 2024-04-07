#include <iostream>
#include <queue>
#include "tree.hpp"
#include "bstream.hpp"

typedef std::priority_queue<tree*, std::vector<tree*>, tree> minheap;

void get_byte_frequency(std::string filename, int* frequency_array)
{
	std::ifstream fin(filename);
	char c;
	std::string data;

	if (!fin) {
		std::cout<<"Unable to open file";
		return;
	}
	while(fin.get(c))
	{
		frequency_array[c] += 1;
		data = data + c;
	}

	fin.close();
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

void encode(std::string filename, tree* huffman)
{
	std::ifstream fin(filename);
	obstream bout(filename+".huf");

	std::string meta = huffman->to_string();
	for(auto x : meta)
	{
		if(x != '0' && x != '1')
			bout.write_byte(x);
		else
			bout.write_bit(x == '1');
	}
	bout.write_bit(0);

	char c;
	while(fin.get(c))
	{
		std::string bitstring = (*huffman)[c];
		for(auto bit : bitstring)
			bout.write_bit(bit == '1');
	}
	fin.close();
	bout.close();
}

int main(int argc, char const *argv[])
{
	int frequency_array[256] = {0};

	// READ FILE
	get_byte_frequency("result.txt", frequency_array);
	// string data = "A_DEAD_DAD_CEDED_A_BAD_BABE_A_BEADED_ABACA_BED";

	// GROW FOREST :P
	minheap forest;
	for(int i = 0; i < 256; i++)
	{
		if(frequency_array[i])
			forest.push(new tree(std::string(1, i), NULL, NULL, frequency_array[i]));
	}

	// CREATE HUFFMAN TREE
	tree* huffman = optimal_merge(forest);
	tree::inorder(huffman);

	std::cout<<huffman->to_string()<<std::endl;
	encode("result.txt", huffman);

	return 0;
}
