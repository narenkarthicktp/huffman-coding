#ifndef TREE_H
#define TREE_H

#include <string>

#define MAX   256
#define LEFT  "0"
#define RIGHT "1"

class tree
{
public:
	std::string data;
	int frequency;
	tree* left;
	tree* right;

	tree(std::string = "", tree* left = NULL, tree* right = NULL, int frequency = 1);
	static void inorder(tree* curr);

	bool operator()(tree* a, tree* b);
	std::string operator[](char key);
	char operator[](std::string path);

	std::string to_string();
};

#endif // TREE_H
