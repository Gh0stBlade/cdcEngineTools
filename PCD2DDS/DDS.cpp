#include "DDS.h"

#include <iostream>
#include <fstream>

void ConvertDDSToPCD(const char* filePath)
{
	std::ifstream ifs(filePath, std::ios::binary);

	if (!ifs.good())
	{
		ifs.close();
		return;
	}

	int fileSize = 0;
	ifs.seekg(0, SEEK_END);
	fileSize = ifs.tellg();
	ifs.seekg(0, SEEK_SET);

	//Load texture data header
	char* fileBuffer = new char[sizeof(DDSHeader)];
	ifs.read(fileBuffer, sizeof(DDSHeader));
	DDSHeader* ddsHeader = nullptr;
	ddsHeader = (DDSHeader*)fileBuffer;
	char* textureData = new char[fileSize-0x80];
	ifs.seekg(0x80, SEEK_SET);
	ifs.read(textureData, fileSize - 0x80);
	ifs.close();

	char nameBuff[128];
	memset(nameBuff, 0, 128);
	sprintf_s(nameBuff, "%s%s", filePath, ".pcd");

	std::ofstream ofs(nameBuff, std::ios::binary);

	unsigned int pcdMagic = 0x39444350;
	unsigned int pcdFormat = ddsHeader->m_format;
	unsigned int pcdTextureDataSize = fileSize - 0x80;
	unsigned int pcdPaletteDataSize = 0;
	unsigned short pcdWidth = ddsHeader->m_width;///@TODO assert max unsigned short
	unsigned short pcdHeight = ddsHeader->m_height;///@TODO assert max unsigned short
	unsigned char pcdDepth = ddsHeader->m_depth;///@TODO assert max unsigned char
	unsigned char pcdMipCount = ddsHeader->m_mipCount;///@TODO assert max unsigned char
	unsigned short pcdFlags = 0x3; //??FILTER_ANISOTROPIC_1X = 0x3,

	//Write section header
	unsigned int sectionMagic = 0x54434553;
	unsigned int sectionFileSize = pcdTextureDataSize + 0x18;
	unsigned int Type = 0x5;
	unsigned int headerSize = 0;

	//
	std::string path(filePath);
	std::string filename;

	size_t pos = path.find_last_of("\\");
	if (pos != std::string::npos)
	{
		filename.assign(path.begin() + pos + 1, path.end());
		pos = path.find_last_of("_");
		filename.assign(path.begin() + pos + 1, path.end());
		filename.erase(filename.find_first_of("."), std::string::npos);
	}
	else
	{
		filename = path;
	}

	unsigned int hash;
	sscanf(filename.c_str(), "%x", &hash);

	unsigned int lang = 0xFFFFFFFF;
	ofs.write((char*)&sectionMagic, sizeof(unsigned int));
	ofs.write((char*)&sectionFileSize, sizeof(unsigned int));
	ofs.write((char*)&Type, sizeof(unsigned int));
	ofs.write((char*)&headerSize, sizeof(unsigned int));
	ofs.write((char*)&hash, sizeof(unsigned int));
	ofs.write((char*)&lang, sizeof(unsigned int));

	ofs.write((char*)&pcdMagic, sizeof(unsigned int));
	ofs.write((char*)&pcdFormat, sizeof(unsigned int));
	ofs.write((char*)&pcdTextureDataSize, sizeof(unsigned int));
	ofs.write((char*)&pcdPaletteDataSize, sizeof(unsigned int));

	ofs.write((char*)&pcdWidth, sizeof(unsigned short));
	ofs.write((char*)&pcdHeight, sizeof(unsigned short));

	ofs.write((char*)&pcdDepth, sizeof(unsigned char));
	ofs.write((char*)&pcdMipCount, sizeof(unsigned char));
	ofs.write((char*)&pcdFlags, sizeof(unsigned short));

	ofs.write(textureData, fileSize - 0x80);

	ofs.flush();
	ofs.close();
	delete[] fileBuffer;
	delete[] textureData;
}