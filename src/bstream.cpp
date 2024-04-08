#include "bstream.hpp"
#include <ios>

bstream::bstream(std::string filename, std::ios_base::openmode flags)
{
	fd.open(filename, flags | std::ios::binary);
	buffer = 0;
	bitcounter = 0;
}
bool bstream::is_open()
{
	return fd.is_open();
}
void bstream::clear_buffer()
{
	buffer = 0;
	bitcounter = 0;
}
void bstream::close()
{
	if(is_open())
		fd.close();
}
bstream::~bstream()
{
	close();
}

ibstream::ibstream(std::string filename, std::ios_base::openmode flags) : bstream(filename, flags | std::ios::in){;}

bool ibstream::eof()
{
	return fd.eof();
}
char ibstream::read_byte()
{
	if(is_open() && !eof())
	{
		if(bitcounter == 0)
			fd.read(&buffer, 1);
		else
		{
			short x = bitcounter;
			char temp = buffer;
			bitcounter = 0;
			while(x--)
			{
				temp <<= 1;
				if(read_bit())
					temp++;
			}
			return temp;
		}
		return buffer;
	}
	return NULL;
}
bool ibstream::read_bit()
{
	if(is_open() && !eof())
	{
		if(!bitcounter)
			fd.read(&buffer, 1);
		char mask = 1 << (7 - bitcounter);
		bitcounter++;
		bitcounter %= 8;
		return buffer & mask;
	}
	return false;
}

obstream::obstream(std::string filename, std::ios_base::openmode flags) : bstream(filename, flags | std::ios::out){;}

void obstream::write_byte(char byte)
{
	if(is_open())
	{
		if(bitcounter == 0)
		{
			buffer = byte;
			fd.write(&buffer, 1);
			return;
		}
		short residualcounter = 8;
		while(bitcounter)
		{
			write_bit(byte & 1<<7);
			byte <<= 1;
			residualcounter--;
		}
		buffer = byte >> (8 - residualcounter);
		bitcounter = residualcounter;
	}
}
void obstream::write_bit(bool bit)
{
	if(is_open())
	{
		buffer <<= 1;
		buffer += bit?1:0;
		if(++bitcounter%8 == 0)
		{
			fd.write(&buffer, 1);
			bitcounter = 0;
		}
	}
}

void obstream::close()
{
	if(is_open())
	{
		while(bitcounter)
			write_bit(0);
		fd.close();
	}
}
obstream::~obstream()
{
	close();
}
