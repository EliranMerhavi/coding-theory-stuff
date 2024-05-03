#pragma once

#include <assert.h>

template<int mod>
class modular_t
{
public:
	modular_t() : m_val(0)
	{
	}

	modular_t(int val) : m_val((val % mod + mod) % mod)
	{
	}

	//bool operator==(const int& b) const {
	//	return m_val == b;
	//}
	//
	//bool operator==(const modular_t& b) const {
	//	return m_val == b.m_val;
	//}

	explicit operator bool() const 
	{
		return m_val != 0;
	}

	modular_t operator+(int b) const
	{
		return *this + modular_t(b);
	}

	modular_t operator-(int b) const
	{
		return *this - modular_t(b);
	}

	modular_t operator*(int b) const
	{
		return *this * modular_t(b);
	}

	modular_t operator/(int b) const
	{
		return *this / modular_t(b);
	}

	modular_t operator+(const modular_t& b) const
	{
		return modular_t(m_val + b.m_val);
	}

	modular_t operator-(const modular_t& b) const
	{
		return modular_t(m_val - b.m_val);
	}

	modular_t operator*(const modular_t& b) const
	{
		return modular_t(m_val * b.m_val);
	}

	modular_t operator/(const modular_t& b) const
	{
		return *this * b.inv();
	}

	modular_t operator-() const 
	{
		return modular_t(-m_val);
	}

	modular_t inv() const
	{
		int x, y;
		assert(gcdExtended(m_val, mod, &x, &y) == 1 && "inverse doenst exists");
		return modular_t(x);
	}

	static modular_t pow(modular_t x, size_t n)
	{
		modular_t result = 1;
		while (n > 0) {
			if ((n & 1) == 1) // y is odd
			{
				result = result * x;
			}
			x = x * x;
			n = n >> 1; // y=y/2;
		}
		return result;
	}
	
	int val() const { return m_val; }

	friend std::ostream& operator<<(std::ostream& os, const modular_t& val)
	{
		return os << val.val();
	}

private:
	int m_val;

	// Function for extended Euclidean Algorithm
	static int gcdExtended(int a, int b, int* x, int* y)
	{
		// Base Case
		if (a == 0) {
			*x = 0, *y = 1;
			return b;
		}

		// To store results of recursive call
		int x1, y1;
		int gcd = gcdExtended(b % a, a, &x1, &y1);

		// Update x and y using results of recursive
		// call
		*x = y1 - (b / a) * x1;
		*y = x1;

		return gcd;
	}
};
