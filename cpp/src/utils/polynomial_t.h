#pragma once
#include <vector>

#include "debug.h"

template<typename val_t>
class polynomial_t
{
public:
	polynomial_t() : m_coefficients() 
	{
	}

	explicit polynomial_t(const std::vector<val_t>& coefficients)
		: m_coefficients(coefficients)
	{
	}

	bool is_zero() const
	{	
		return m_coefficients.size() == 1 && !m_coefficients.front();
	}

	val_t operator()(const val_t& x) const
	{
		int n = m_coefficients.size();
		val_t res = 0;

		for (int i = n - 1; i >= 0; i--) {
			res = res * x + m_coefficients[i];
		}
		
		return res;
	}

	const std::vector<val_t>& coefficients() const
	{
		return m_coefficients;
	}

	std::pair<polynomial_t, polynomial_t> divmod(polynomial_t& G)
	{
		// REF: https://stackoverflow.com/questions/26173058/division-of-polynomials-in-python
		// translated to algorithm posted here to c++

		normalize(m_coefficients);
		normalize(G.m_coefficients);

		if (m_coefficients.size() < G.m_coefficients.size()) {
			return { polynomial_t({0}),  polynomial_t(*this) };
		}

		int shiftlen = m_coefficients.size() - G.m_coefficients.size();

		std::vector<val_t> den(shiftlen, 0);
		std::vector<val_t> num = m_coefficients;
		
		den.insert(den.end(), G.m_coefficients.begin(), G.m_coefficients.end());

		std::vector<val_t> quot;
		val_t divisor = den.back();

		for (int i = 0; i <= shiftlen; i++) {
			val_t mult = num.back() / divisor;
			quot.push_back(mult);
			if (mult) {
				for (int i = 0; i < num.size(); i++) {
					num[i] = num[i] - mult * den[i];
				}
			}
			num.pop_back();
			den.erase(den.begin());
		}

		std::reverse(quot.begin(), quot.end());
		normalize(num);
		return { polynomial_t{quot} , polynomial_t{num} };
	}

	friend std::ostream& operator<<(std::ostream& os, polynomial_t& P) 
	{
		polynomial_t::normalize(P.m_coefficients);
		const auto& coef = P.coefficients();
		int n = coef.size();

		if (n == 1) {
			return os << coef[0];
		}

		if (n == 2) {
			return os << coef[1] << "*x + " << coef[0];
		}
		
		for (int i = n - 1; i > 1; i--) {
			os << coef[i] << "*x^" << i << " + ";
		}

		return os << coef[1] << "*x + " << coef[0];
	}

private:
	std::vector<val_t> m_coefficients;
private:

	static void normalize(std::vector<val_t>& coefficients)
	{
		while (coefficients.size() && !coefficients.back()) {
			coefficients.pop_back();
		}
		if (coefficients.empty()) {
			coefficients.push_back(0);
		}
	}
};