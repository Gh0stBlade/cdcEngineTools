#include "File.h"

#include <iostream>
#include <fstream>

unsigned int getFileMagic(const char* filePath)
{
	std::ifstream ifs(filePath, std::ios::binary);
	unsigned int fileMagic = 0;

	if (ifs.good())
	{
		
		ifs.read((char*)&fileMagic, sizeof(unsigned int));

		if (fileMagic == 0x54434553)
		{
			//Skip past section header
			ifs.seekg(0x14, SEEK_CUR);

			ifs.read((char*)&fileMagic, sizeof(unsigned int));
			return fileMagic;
		}
	}

	return fileMagic;
}