#include "pch.h"
#include "bit_stream.h"
#include "debug.h"

bit_stream::bit_stream(const std::string& buffer) : m_bit_pos(-1), m_buffer(buffer)
{
}

void bit_stream::write_bits(char buff, int bits_count)
{
	for (int i = 0; i < bits_count; i++)
	{
		assure_capacity();
		this->m_bit_pos++;
		(m_buffer[this->m_bit_pos / 8] |= ((buff >> i) & 1) << (this->m_bit_pos % 8));
	}
}

void bit_stream::read_bits(char &buff, int bits_count)
{
	buff = 0;
	assert(this->m_bit_pos + bits_count < this->bit_cap(), " trying to read after end of buffer");
	
	bits_count = std::min(bits_count, 8);

	for (int i = 0; i < bits_count; i++)
	{
		this->m_bit_pos++;
		buff |= ((m_buffer[this->m_bit_pos / 8] >> (this->m_bit_pos % 8)) & 1) << i;
	}
}


void bit_stream::write(size_t buff)
{
	for (size_t byte_i = 0; byte_i < sizeof(buff); byte_i++)
		write_bits(char(buff >> (8 * byte_i)), 8);
}

void bit_stream::read(size_t& buff)
{
	char temp;
	buff = 0;
	for (size_t byte_i = 0; byte_i < sizeof(buff); byte_i++)
	{
		read_bits(temp, 8);
		buff |= (size_t)(unsigned char)temp << (8 * byte_i);
	}
}

bit_stream& bit_stream::operator<<(bool bit)
{	
	assure_capacity();
	write_bits(bit, 1);
	return *this;
}

bit_stream& bit_stream::operator>>(bool& bit)
{
	assert(this->m_bit_pos <= this->bit_cap(), " trying to read after end of buffer");
	read_bits(reinterpret_cast<char&>(bit), 1);
	return *this;
}

void bit_stream::set_buffer(const std::string& buffer)
{
	std::move(buffer.begin(), buffer.end(), std::back_inserter(m_buffer));
}



void bit_stream::assure_capacity()
{
	if (this->m_bit_pos + 1 >= this->bit_cap())
		this->m_buffer += '\0';
}

