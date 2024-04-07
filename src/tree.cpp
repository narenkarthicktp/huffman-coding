#include "tree.hpp"
#include <iostream>

tree::tree(std::string data, tree* left, tree* right, int frequency)
{
	this->data = data;
	this->left = left;
	this->right = right;
	this->frequency = frequency;
}

void tree::inorder(tree* curr)
{
	if(curr == NULL) return;
	inorder(curr->left);
	std::cout<<curr->data<<" "<<curr->frequency<<std::endl;
	inorder(curr->right);
}
bool tree::operator()(tree* a, tree* b)
{
	return a->frequency > b->frequency;
}

std::string tree::operator[](char target)
{
	std::string buffer = "";

	if(this->left && this->left->data.find(target) != std::string::npos)
		buffer = LEFT  + (*this->left)[target];
	else if(this->right && this->right->data.find(target) != std::string::npos)
		buffer = RIGHT + (*this->right)[target];

	return buffer;
}
char tree::operator[](std::string bitstring)
{
	tree* curr = this;
	for(auto i : bitstring)
	{
		if(!curr) return NULL;
		curr = i-'0'?curr->right:curr->left;
	}
	return curr->left && curr->right ? NULL : curr->data[0];
}

std::string tree::to_string()
{
	std::string meta = "";
	if(!this->left && !this->right)
		return "1"+this->data;
	if(this->left)
		meta += this->left->to_string();
	if(this->right)
		meta += this->right->to_string();
	return meta+"0";
}
