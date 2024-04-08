#include "decoder.hpp"
#include "bstream.hpp"
#include <iostream>
#include <stack>

tree* reconstruct_tree(ibstream* bin)
{
	if(!bin || !(bin->is_open()))
		return NULL;

	std::stack<tree*> forest;
	while(true)
	{
		if(bin->read_bit())
			forest.push(new tree(std::string(1, bin->read_byte())));
		else if(forest.size() == 1)
			break;
		else
		{
			tree* right = forest.top(); forest.pop();
			tree* left = forest.top(); forest.pop();
			forest.push(new tree(left->data+right->data, left, right));
		}
	}
	return forest.top();
}

void read_prefix_codes(tree* huffman, ibstream* bin, std::ostream* out)
{
	if(!out || !bin || !(bin->is_open()))
		return;

	std::string path_buffer = "";
	while(true)
	{
		path_buffer += (char)('0' + (int)bin->read_bit());
		if(bin->eof())
			break;
		char x = (*huffman)[path_buffer];
		if(x)
		{
			path_buffer = "";
			out->write(&x, 1);
		}
	}
}

void decode(std::string source_file, std::string target_file)
{
	ibstream bin(source_file);
	std::ofstream fout(target_file);

	tree* huffman = reconstruct_tree(&bin);
	read_prefix_codes(huffman, &bin, &std::cout);

	fout.close();
	bin.close();
	return;
}
