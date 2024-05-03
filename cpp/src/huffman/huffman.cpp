#include "pch.h"
#include "huffman.h"
#include "debug.h"
#include "utils/bit_stream.h"
#include "huffman_tree.h"

namespace huffman
{
	void build_huffman_code(const huffman_tree::node* root, std::string& current_path, std::vector<bool>(&huffman_code)[UCHAR_MAX])
	{
		if (root->is_value_node)
		{
			auto& code_for_char = huffman_code[(unsigned char)root->value];
			for (char bit : current_path)
				code_for_char.emplace_back(bit == '1');
			return;
		}
		
		current_path += '0';
		build_huffman_code(root->left, current_path, huffman_code);
		current_path.pop_back();
		current_path += '1';
		build_huffman_code(root->right, current_path, huffman_code);
		current_path.pop_back();
	}

	void dump_huffman_tree(const huffman_tree::node* root, bit_stream& bs)
	{
		if (root)
		{
			dump_huffman_tree(root->left, bs);
			dump_huffman_tree(root->right, bs);
			if (root->is_value_node)
			{
				bs << 1;
				bs.write_bits(root->value, 8);
			}
			else
			{
				bs << 0;
			}
		}
	}

	huffman_tree::node* load_huffman_tree(bit_stream& bs)
	{
		std::stack<huffman_tree::node*> stk;
		bool bit;
		char value;
		while (true)
		{
			bs >> bit;
			if (bit)
			{
				bs.read_bits(value, 8);
				stk.push(new huffman_tree::node{ nullptr, nullptr, 0, value, true });
			}
			else if (stk.size() == 1)
			{
				break;
			}
			else
			{
				huffman_tree::node* right = stk.top();
				stk.pop();
				huffman_tree::node* left = stk.top();
				stk.pop();
				stk.push(new huffman_tree::node{ left, right, 0, 0, false });
			}
		}

		return stk.top();
	}

	void print_tree(const huffman_tree::node* root, int indent = 0, bool is_left=false)
	{
		if (root)
		{
			for (int i = 0; i < indent; i++) std::cout << ' ';
			std::cout << !is_left << "->";
			if (root->is_value_node)
				std::cout << root->value;
			std::cout << '\n';
			print_tree(root->left, indent + 1, true);
			print_tree(root->right, indent + 1, false);
			
		}
	}

	std::string compress(const std::string& text)
	{
		huffman_tree::node* root;
		std::vector<bool> huffman_code[UCHAR_MAX];
		size_t freq[UCHAR_MAX]{ 0 };
	
		for (unsigned char ch : text)
			freq[ch]++;
		
		huffman_tree::build(freq, root);
		std::string path;
		build_huffman_code(root, path, huffman_code);

		bit_stream main_bs, compressed_text_bs_holder;

		/*
		* compresses the text using 
		* the huffman code
		*/
		for (unsigned char ch : text)
		{
			for (bool bit : huffman_code[(unsigned char)ch])
			{
				compressed_text_bs_holder << bit;
			}
		}

		dump_huffman_tree(root, main_bs);
		main_bs << 0;

		// write number of bits of the compressed text	
		main_bs.write(compressed_text_bs_holder.bit_pos() + 1);

		// write compressed text to the main byte stream
		for (auto byte : compressed_text_bs_holder.buffer())
			main_bs.write_bits(byte, 8);
		
		huffman_tree::free(root);
		return main_bs.buffer();
	}

	std::string decompress(const std::string& compressed_text)
	{
		bit_stream bs(compressed_text);
		std::string res;
		huffman_tree::node* root, * curr_node;
		size_t freq[UCHAR_MAX]{ 0 }, number_of_bits;

		root = load_huffman_tree(bs);
		
		bs.read(number_of_bits);
		
		curr_node = root;
		
		bool bit;

		// traverse the huffman tree from root with the compressed text bits.
		// when reaching a value node, we add it to the result and traverse from root again
		for (size_t curr_bit = 0; curr_bit < number_of_bits; curr_bit++)
		{
			bs >> bit;
			curr_node = bit ? curr_node->right : curr_node->left;
			if (curr_node->is_value_node)
			{
				res += curr_node->value;
				curr_node = root;
			}
		}
	
		huffman_tree::free(root);
		return res;
	}
}