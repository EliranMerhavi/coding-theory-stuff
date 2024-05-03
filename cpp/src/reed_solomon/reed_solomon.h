#pragma once

#include <string>
#include <vector>

#include "utils/modular_t.h"

namespace reed_solomon
{
	using val_t = modular_t<2087>;
//	std::string encode(const std::string& message);
	std::string encode(const std::string& message);
	std::string decode(const std::string& codeword);
//	std::string decode(const std::string& codeword);
}