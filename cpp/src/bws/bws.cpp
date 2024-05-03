#include "bws.h"
#include <vector>
#include <algorithm>
#include <iostream>

std::wstring bws::transform(const std::string& text)
{
	
	int n = text.size() + 2;
	std::wstring wtext(text.begin(), text.end());	
	std::vector<std::wstring> table(n);

	wtext = START_TEXT + wtext + END_TEXT;

	for (int i = 0; i < n; i++) 
	{
		wchar_t ch = wtext.back();
		wtext.pop_back();
		wtext = ch + wtext;
		table[i] = wtext;
	}

	std::wstring res;
	
	std::sort(table.begin(), table.end());

	for (int i = 0; i < n; i++) 
	{
		res += table[i].back();
	}
	
	return res;
}

std::string bws::inverse_transform(const std::wstring& text)
{
	int n = text.size(), i;
	std::vector<std::wstring> table(n, std::wstring(n, L' '));

	for (i = n - 1; i >= 0; i--) 
	{
		for (int j = 0; j < n; j++) 
			table[j][i] = text[j];
		std::sort(table.begin(), table.end());
	}

	for (i = 0; i < n && table[i][0] != START_TEXT; i++)
		;
	
	std::string res;

	for (int j = 1; j < n - 1; j++) 
	{
		res += table[i][j];
	}

	return res;
}
