#include "encoder.hpp"
#include "decoder.hpp"
#include <cstring>
using namespace std;

int main(int argc, char const *argv[])
{
	string source_file, target_file;
	switch(argc)
	{
		case 4:
			if(!strcmpi(argv[1], "-e"))
			{
				source_file = string(argv[2]);
				target_file = string(argv[3]);
				encode(source_file, target_file);
				break;
			}
			else if(!strcmpi(argv[1], "-d"))
			{
				source_file = string(argv[2]);
				target_file = string(argv[3]);
				decode(source_file, target_file);
				break;
			}
	}
	return 0;
}
