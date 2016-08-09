#include "DDS.h"
#include "File.h"

#include <iostream>
#include <fstream>

void ConvertDDSToPCD(const char* filePath)
{
	std::ifstream ifs(filePath, std::ios::binary);

	//If there was a failure to open the file we must exit
	if (!ifs.good())
	{
		std::cout << "Error: failed to open the file!" << std::endl;
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
	if (ddsHeader->m_magic != DDS_MAGIC)
	{
		std::cout << "Error: DDS magic mis-match!" << std::endl;
		ifs.close();
		delete[] fileBuffer;
		return;
	}

	char* textureData = new char[fileSize-0x80];
	ifs.seekg(0x80, SEEK_SET);
	ifs.read(textureData, fileSize - 0x80);
	ifs.close();

	char nameBuff[128];
	memset(nameBuff, 0, 128);
	sprintf_s(nameBuff, "%s%s", filePath, ".pcd");

	std::ofstream ofs(nameBuff, std::ios::binary);

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

	WriteUInt(ofs, SECTION_MAGIC);
	WriteUInt(ofs, ((fileSize-0x80) + 0x18));
	WriteUInt(ofs, (TEXTURE_SECTION_TYPE));
	WriteUInt(ofs, 0);
	WriteUInt(ofs, hash);
	WriteUInt(ofs, 0xFFFFFFFF);

	WriteUInt(ofs, PCD_MAGIC);
	WriteUInt(ofs, ddsHeader->m_format);
	WriteUInt(ofs, (fileSize - 0x80));
	WriteUInt(ofs, 0);
	WriteUShort(ofs, ddsHeader->m_width);
	WriteUShort(ofs, ddsHeader->m_height);
	WriteUByte(ofs, ddsHeader->m_depth);
	WriteUByte(ofs, ddsHeader->m_mipCount);
	WriteUShort(ofs, 3);

	ofs.write(textureData, (fileSize - 0x80));

	ofs.flush();
	ofs.close();
	delete[] fileBuffer;
	delete[] textureData;
}