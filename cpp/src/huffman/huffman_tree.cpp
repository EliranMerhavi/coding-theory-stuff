#include "huffman_tree.h"
#include <stack>
#include <queue>

void huffman_tree::free(huffman_tree::node* root)
{
	std::stack<huffman_tree::node*> dfs;
	dfs.push(root);

	while (!dfs.empty())
	{
		huffman_tree::node* n = dfs.top();
		dfs.pop();
		if (n->left)
			dfs.push(n->left);
		if (n->right)
			dfs.push(n->right);
		delete n;
	}
}

void huffman_tree::build(const size_t(&freq)[UCHAR_MAX], huffman_tree::node*& res)
{
	std::priority_queue<huffman_tree::node*, std::vector<huffman_tree::node*>, lesscmp> q;

	for (int i = 0; i < UCHAR_MAX; i++)
		if (freq[i])
			q.push(new huffman_tree::node{ nullptr, nullptr, freq[i], (char)i, true });

	if (q.empty())
	{
		res = nullptr;
		return;
	}

	while (q.size() > 1)
	{
		huffman_tree::node* a = q.top();
		q.pop();
		huffman_tree::node* b = q.top();
		q.pop();

		q.push(new huffman_tree::node{ a, b, a->freq + b->freq, 0, false });
	}

	res = q.top();
}