#include "decoder.hpp"
#include "bstream.hpp"

tree* reconstruct_tree(ibstream* bin)
{
	// if(!bin)
		// return NULL;

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
	std::string path_buffer = "";
	while(true)
	{
		path_buffer += (char)('0' + (int)bin->read_bit());
		if(bin->eof())
			break;
		char x = (*huffman)[path_buffer];
		if(x)
		{
			// std::cout<<x;
			path_buffer = "";
			out->write(&x, 1);
		}
	}
}

void decode(std::string filename)
{
	// ofstream fout(filename);
	ibstream bin(filename+".huf");
	tree* huffman = reconstruct_tree(&bin);
	read_prefix_codes(huffman, &bin, &std::cout);
	return;
}
