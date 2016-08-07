#include "PCD.h"

#include <iostream>
#include <fstream>

void ConvertPCDToDDS(const char* filePath)
{
	std::ifstream ifs(filePath, std::ios::binary);

	//If there was a failure to open the file we must exit
	if (!ifs.good())
	{
		ifs.close();
		return;
	}

	//Skip past section header
	ifs.seekg(0x18, SEEK_SET);

	//Load texture data header
	char* fileBuffer = new char[sizeof(PCTextureDataHeader)];
	ifs.read(fileBuffer, sizeof(PCTextureDataHeader));
	PCTextureDataHeader* pcdHeader = nullptr;
	pcdHeader = (PCTextureDataHeader*)fileBuffer;
	char* textureData = new char[pcdHeader->m_textureDataSize];
	ifs.read(textureData, pcdHeader->m_textureDataSize);
	ifs.close();

	char nameBuff[128];
	memset(nameBuff, 0, 128);
	sprintf_s(nameBuff, "%s%s", filePath, ".dds");

	std::ofstream ofs(nameBuff, std::ios::binary);

	unsigned int ddsMagic = 0x20534444;
	unsigned int ddsSize = 0x7C;
	unsigned int ddsFlags = 0x000A1007;
	unsigned int ddsPitchOrLinearSize = 0x00010000;
	unsigned int ddsDummy = 0;
	unsigned int ddsHeight = pcdHeader->m_height;
	unsigned int ddsWidth = pcdHeader->m_width;
	unsigned int ddsDepth = pcdHeader->m_depth;
	unsigned int ddsMipCount = pcdHeader->m_numMipMaps;
	unsigned int ddsFormat = pcdHeader->m_format;
	unsigned int ddsUnk00 = 0x00401008;

	ofs.write((char*)&ddsMagic, sizeof(unsigned int));
	ofs.write((char*)&ddsSize, sizeof(unsigned int));
	ofs.write((char*)&ddsFlags, sizeof(unsigned int));
	ofs.write((char*)&ddsHeight, sizeof(unsigned int));

	ofs.write((char*)&ddsWidth, sizeof(unsigned int));
	ofs.write((char*)&ddsPitchOrLinearSize, sizeof(unsigned int));
	ofs.write((char*)&ddsDummy, sizeof(unsigned int));
	ofs.write((char*)&ddsDepth, sizeof(unsigned int));

	ofs.write((char*)&ddsMipCount, sizeof(unsigned int));

	//Reserved
	ofs.seekp(12 * sizeof(unsigned int), SEEK_CUR);

	ofs.write((char*)&ddsFormat, sizeof(unsigned int));
	ofs.seekp(0x14, SEEK_CUR);
	ofs.write((char*)&ddsUnk00, sizeof(unsigned int));
	ofs.seekp(0x10, SEEK_CUR);

	ofs.write(textureData, pcdHeader->m_textureDataSize);

	ofs.flush();
	ofs.close();
	delete[] fileBuffer;
	delete[] textureData;
}