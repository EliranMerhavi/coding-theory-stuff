#pragma once


class bit_stream
{
public:
	using bit_pos_t = long long;
	bit_stream(const std::string& buffer = "");

	void read_bits(char& buff, int bits_count);
	void read(size_t& buff);

	void write_bits(char buff, int bits_count);
	void write(size_t buff);
	bit_stream& operator<<(bool bit);
	bit_stream& operator>>(bool& bit);
	
	void set_buffer(const std::string& buffer);
	inline const std::string& buffer() const { return m_buffer; }

	inline void rewind() { this->m_bit_pos = -1; }
	inline bool end() const { return this->m_bit_pos == bit_cap() - 1; }
	
	inline const bit_pos_t& bit_pos() const { return m_bit_pos; }

private:
	bit_pos_t m_bit_pos;
	std::string m_buffer;

	void assure_capacity();
	inline bit_pos_t bit_cap() const { return m_buffer.size() * 8; }
};