#pragma once
#include <string>

/*
* Burrows-Wheeler transorm
*/

#define START_TEXT L'\xff01'
#define END_TEXT L'\xff02'


namespace bws
{
	std::wstring transform(const std::string& text);
	std::string inverse_transform(const std::wstring& text);
}