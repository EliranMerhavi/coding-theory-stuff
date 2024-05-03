#pragma once

namespace huffman
{
	std::string compress(const std::string& text);
	std::string decompress(const std::string& compressed_text);
}
