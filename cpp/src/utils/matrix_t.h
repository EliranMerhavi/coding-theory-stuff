#pragma once

#include <vector>

template<typename val_t>
using vector = std::vector<val_t>;

template<typename val_t>
using matrix_t = vector<vector<val_t>>;

template<typename val_t>
void print(const matrix_t<val_t>& mat);

template<typename val_t>
vector<val_t> gussian_elimination(matrix_t<val_t>& mat)
{
	int m = mat.size(), n = mat[0].size();

	for (int k = 0; k < m; k++) {

		int i_max = k;

		for (int i = k + 1; i < m; i++) {
			if (abs(mat[i][k].val()) > abs(mat[i_max][k].val()))
				i_max = i;
		}

		if (!mat[i_max][k].val()) {
			return {};
		}

		if (i_max != k) {
			// swap rows
			for (int i = 0; i < n; i++) {
				std::swap(mat[k][i], mat[i_max][i]);
			}
		}

		val_t x = mat[k][k];

		for (int i = k + 1; i < m; i++) {
			val_t y = mat[i][k];
			for (int j = k + 1; j < n; j++) {
				mat[i][j] = x * mat[i][j] - y * mat[k][j];
			}
			mat[i][k] = 0;
		}
	}
	
	for (int k = m - 1; k >= 0; k--) {
		val_t x = mat[k][k];
		for (int i = k - 1; i >= 0; i--) {
			val_t y = mat[i][k];
			for (int j = 0; j < n; j++) {
				mat[i][j] = mat[i][j] = x * mat[i][j] - y * mat[k][j];;
			}
		}
	}

	vector<val_t> res(m);
	
	for (int i = 0; i < m; i++) {
		mat[i][n - 1] = mat[i][n - 1] / mat[i][i];
		mat[i][i] = 1;
		res[i] = mat[i][n - 1];
	}
	
	return res;
}