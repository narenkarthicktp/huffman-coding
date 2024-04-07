#ifndef BSTREAM_HPP
#define BSTREAM_HPP

#include <iostream>
#include <fstream>

class bstream
{
protected:
	std::fstream fd;
	char buffer;
	short bitcounter;
public:
	bstream(std::string filename, std::ios_base::openmode flags = std::ios::binary);
	void close();
	void clear_buffer();
	~bstream();
};

class ibstream : public bstream
{
public:
	ibstream(std::string filename, std::ios_base::openmode flags = std::ios::binary | std::ios::in);
	bool eof();
	bool read_bit();
	char read_byte();
};

class obstream : public bstream
{
public:
	obstream(std::string filename, std::ios_base::openmode flags = std::ios::binary | std::ios::out);
	void write_bit(bool bit);
	void write_byte(char byte);
	void close();
	~obstream();
};


#endif // BSTREAM_HPP
