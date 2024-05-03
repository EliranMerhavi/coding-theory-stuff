#pragma once

#include <limits>

namespace huffman_tree
{
	struct node
	{
		node* left;
		node* right;
		size_t freq;
		char value;
		bool is_value_node;
	};

	void free(huffman_tree::node* root);
	void build(const size_t(&freq)[UCHAR_MAX], huffman_tree::node*& res);

	struct lesscmp
	{
		bool operator()(const huffman_tree::node* a, const huffman_tree::node* b) const
		{
			return a->freq > b->freq;
		}
	};
}
