#pragma once
#include "utils/matrix_t.h"


template<typename type>
void print_in_binary(type value)
{
	std::cout << "0b";
	for (int i = sizeof(value) * 8 - 1; i >= 0; i--)
	{
		std::cout << (((type)value >> i) & 1);
	}
	std::cout << '\n';
}

template<typename val_t>
void print(const matrix_t<val_t>& mat) {
	int m = mat.size(), n = mat[0].size();

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			std::cout << mat[i][j] << '\t';
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

template<typename val_t>
void print(const std::vector<val_t>& v) {
	int m = v.size();

	for (int j = 0; j < m; j++) {
		std::cout << v[j] << ' ';
	}

	std::cout << '\n';
}
