#include "PCD.h"
#include "File.h"

#include <iostream>
#include <fstream>

void ConvertPCDToDDS(const char* filePath)
{
	std::ifstream ifs(filePath, std::ios::binary);

	//If there was a failure to open the file we must exit
	if (!ifs.good())
	{
		std::cout << "Error: failed to open the file!" << std::endl;
		ifs.close();
		return;
	}

	//Check to see if file has valid section header
	unsigned int sectionMagic = ReadUInt(ifs);
	if (sectionMagic != SECTION_MAGIC)
	{
		std::cout << "Error: section magic mis-match!" << std::endl;
		ifs.close();
		return;
	}

	unsigned int sectionSize = ReadUInt(ifs);
	if (sectionSize <= 0)
	{
		std::cout << "Error: section corrupt, size is <= 0!" << std::endl;
		ifs.close();
		return;
	}

	unsigned char sectionType = ReadUByte(ifs);
	if (sectionType != TEXTURE_SECTION_TYPE)
	{
		std::cout << "Error: invalid section type!" << std::endl;
		ifs.close();
		return;
	}

	//Skip past section header
	ifs.seekg(24, SEEK_SET);

	//Load texture data header
	char* fileBuffer = new char[sizeof(PCTextureDataHeader)];
	ifs.read(fileBuffer, sizeof(PCTextureDataHeader));
	
	PCTextureDataHeader* pcdHeader = nullptr;
	pcdHeader = (PCTextureDataHeader*)fileBuffer;
	if (pcdHeader->m_magic != PCD_MAGIC)
	{
		std::cout << "Error PCD magic mis-match!" << std::endl;
		ifs.close();
		return;
	}
	
	char* textureData = new char[pcdHeader->m_textureDataSize];
	ifs.read(textureData, pcdHeader->m_textureDataSize);
	ifs.close();

	char nameBuff[128];
	memset(nameBuff, 0, 128);
	sprintf_s(nameBuff, "%s%s", filePath, ".dds");

	std::ofstream ofs(nameBuff, std::ios::binary);

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

	WriteUInt(ofs, DDS_MAGIC);
	WriteUInt(ofs, ddsSize);
	WriteUInt(ofs, ddsFlags);
	WriteUInt(ofs, ddsHeight);

	WriteUInt(ofs, ddsWidth);
	WriteUInt(ofs, ddsPitchOrLinearSize);
	WriteUInt(ofs, ddsDummy);
	WriteUInt(ofs, ddsDepth);

	WriteUInt(ofs, ddsMipCount);

	//Reserved
	ofs.seekp(12 * sizeof(unsigned int), SEEK_CUR);

	WriteUInt(ofs, ddsFormat);
	ofs.seekp(0x14, SEEK_CUR);
	WriteUInt(ofs, ddsUnk00);
	ofs.seekp(0x10, SEEK_CUR);

	ofs.write(textureData, pcdHeader->m_textureDataSize);

	ofs.flush();
	ofs.close();
	delete[] fileBuffer;
	delete[] textureData;
}