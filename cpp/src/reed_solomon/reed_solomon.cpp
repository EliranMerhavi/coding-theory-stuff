#include "reed_solomon.h"
#include <vector>
#include <assert.h>
#include <iostream>
#include <algorithm>

#include "utils/modular_t.h"
#include "utils/polynomial_t.h"
#include "utils/matrix_t.h"

#include "debug.h"

namespace reed_solomon
{
	constexpr int message_block_length = 223; // K
	constexpr int codeword_block_length = 255; // N

	bool welch_berlekamp(const std::vector<val_t>& b, polynomial_t<val_t>& F);
	
	std::vector<val_t> encode_block(const std::string& message);
	std::string decode_block(const std::vector<val_t>& codeword);
}

std::string reed_solomon::encode(const std::string& message)
{
	//std::cout << std::hex;
	//std::cout << decode_block(encode_block(message)) << '\n';
	size_t size = message.size();
	std::string payload;
	
	for (int i = 0; i < 64; i += 8) {
		payload += (char)(size >> i);
	}

	payload += message;
	
	int padding = (message_block_length - (payload.size() % message_block_length)) % message_block_length;
	
	payload.insert(payload.end(), padding, 'p');

	std::vector<val_t> data;

	std::string block = "";
	std::string test = "";

	for (char ch : payload) {
		block += ch;
		if (block.size() == message_block_length) {
			auto codeword_block = reed_solomon::encode_block(block);
			data.insert(data.end(), codeword_block.begin(), codeword_block.end());
			block.clear();
		}
	}
	
	std::string codeword = "";
	
	for (const val_t& y : data) {
		codeword += (char)(y.val() & 0xFF);
		codeword += (char)(y.val() >> 8);
	}
	
	return codeword;
}

std::string reed_solomon::decode(const std::string& codeword)
{
	std::vector<val_t> data;
	
	for (int i = 0; i < codeword.size(); i += 2) {
		data.emplace_back((uint16_t)(uint8_t)codeword[i] + ((uint16_t)(uint8_t)codeword[i + 1] << 8));
	}

	std::vector<val_t> block;
	std::string message = "";

	for (const val_t& y : data) {
		block.push_back(y);
		if (block.size() == codeword_block_length) {
			message += reed_solomon::decode_block(block);
			block.clear();
		}		
	}

	size_t size = 0;

	for (int i = 0; i < 64; i += 8) 
	{
		size += (size_t)(uint8_t)message[i / 8] << i;
	}
	
	return message.substr(8, size);
}


std::vector<reed_solomon::val_t> reed_solomon::encode_block(const std::string& message)
{
	assert(message.size() == message_block_length);
	
	std::vector<val_t> coefficients(message_block_length);

	for (int i = 0; i < message_block_length; i++) {
		coefficients[i] = (uint8_t)message[i];
	}

	polynomial_t<val_t> F(coefficients);
	
	std::vector<val_t> codeword;
	
	for (int ai = 0; ai < codeword_block_length; ai++) {
		codeword.emplace_back(F(ai));
	}
	
	return codeword;
}

std::string reed_solomon::decode_block(const std::vector<val_t>& codeword)
{
	assert(codeword.size() == codeword_block_length);
	polynomial_t<val_t> F;
	
	if (!welch_berlekamp(codeword, F)) {
		return "";
	}
	
	std::string message;
	
	for (val_t c : F.coefficients()) {
		message += c.val();
	}

	message.insert(message.end(), (message_block_length - F.coefficients().size()) % message_block_length, 0);

	return message;
}

bool reed_solomon::welch_berlekamp(const std::vector<val_t>& b, polynomial_t<val_t>& F)
{
	matrix_t<val_t> mat(codeword_block_length, std::vector<val_t>(codeword_block_length + 1));
	
	for (int e = (codeword_block_length - message_block_length) / 2; e >= 0; e--) {
		
		for (int r = 0; r < codeword_block_length; r++) {
#define ai r
			val_t x = 1;
			for (int c = 0; c < e; c++) {
				mat[r][c] = b[r] * x;
				x = x * ai;
			}
			mat[r][codeword_block_length] = -b[r] * x;
			x = 1;
			for (int c = e; c < codeword_block_length; c++) {
				mat[r][c] = -x;
				x = x * ai;
			}
#undef ai
		}
		std::vector<val_t> coefficients = gussian_elimination(mat);
		
		if (coefficients.empty()) {
			continue;
		}
		
		coefficients.insert(coefficients.begin() + e, 1);

		polynomial_t<val_t> E({ coefficients.begin(), coefficients.begin() + (e + 1) });
		polynomial_t<val_t> Q({ coefficients.begin() + (e + 1) , coefficients.end() });

		auto [F1, D] = Q.divmod(E);
		
		if (D.is_zero()) {
			F = F1;
			return true;
		}
	}

	return false;
}