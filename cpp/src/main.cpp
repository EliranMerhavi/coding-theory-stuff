#include "pch.h"

#include "huffman/huffman.h"
#include "bws/bws.h"
#include "reed_solomon/reed_solomon.h"

#include "utils/modular_t.h"
#include "utils/polynomial_t.h"
#include "utils/matrix_t.h"

std::string load_text(const std::string& filepath)
{
	FILE* fp;
	
	if (fopen_s(&fp, filepath.c_str(), "rb"))
	{
		std::cerr << "error when opening file \"" << filepath << "\"\n";
		exit(1);
	}

	fseek(fp, 0, SEEK_END);
	size_t length = ftell(fp);
	rewind(fp);
	char* str = new char[length + 1];
	fread(str, sizeof(char), length, fp);
	fclose(fp);
	str[length] = '\0';
	std::string content = str;
	delete[] str;
	return content;
}

void huffman_test()
{
	std::cout << "huffman\n";
	std::string text = load_text("res/huffman_test.txt");

	auto compressed = huffman::compress(text);

	std::string decompressed_text = huffman::decompress(compressed);
	
	assert(decompressed_text == text);
	
	std::cout << text.size() << " bytes -> " << compressed.size() << " bytes\n";

	std::cout << "huffman tests passed!\n";
}

void bws_test() 
{
	std::cout << "bws\n";
	std::string text = "BANANA";
	
	std::wstring T = bws::transform(text);
	
	std::string res = bws::inverse_transform(T);
	
	assert(res == text);

	std::cout << "bws tests passed!\n";
}

void test() 
{
	huffman_test();
	bws_test();
}

int main()
{
	std::string message = "hello world";

	std::string something = reed_solomon::encode(message);

	std::string decoded = reed_solomon::decode(something);


	std::cout << decoded << '\n';



	return 0;
}
