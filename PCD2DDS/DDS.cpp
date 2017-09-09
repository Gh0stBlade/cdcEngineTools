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

	unsigned int magic;
	ifs.read(reinterpret_cast<char*>(&magic), sizeof(unsigned int));
	ifs.seekg(0, SEEK_SET);

	switch (magic)
	{
	case 0x00020000://Targa
		WritePCTarga(filePath, ifs);
		break;
	case DDS_MAGIC:
		WritePCDDS(filePath, ifs);
		break;
	default:
		std::cout << "Error: Unsupported texture format!" << std::endl;
		break;
	}

	ifs.close();
}

void WritePCDDS(const char* resultFileName, std::ifstream& ifs)
{
	int fileSize = 0;
	ifs.seekg(0, SEEK_END);
	fileSize = ifs.tellg();
	ifs.seekg(0, SEEK_SET);

	DDSHeader ddsHeader;
	ifs.read(reinterpret_cast<char*>(&ddsHeader), sizeof(DDSHeader));

	if (ddsHeader.m_magic != DDS_MAGIC)
	{
		std::cout << "Error: DDS magic mis-match!" << std::endl;
		ifs.close();
		return;
	}

	char* textureData = new char[fileSize - 0x80];
	ifs.seekg(0x80, SEEK_SET);
	ifs.read(textureData, fileSize - 0x80);
	ifs.close();

	char nameBuff[128];
	memset(nameBuff, 0, 128);
	sprintf_s(nameBuff, "%s%s", resultFileName, ".pcd");

	std::ofstream ofs(nameBuff, std::ios::binary);

	//
	std::string path(resultFileName);
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
	WriteUInt(ofs, ((fileSize - 0x80) + 0x18));
	WriteUInt(ofs, (TEXTURE_SECTION_TYPE));
	WriteUInt(ofs, 0);
	WriteUInt(ofs, hash);
	WriteUInt(ofs, 0xFFFFFFFF);

	WriteUInt(ofs, PCD_MAGIC);
	WriteUInt(ofs, ddsHeader.m_format);
	WriteUInt(ofs, (fileSize - 0x80));
	WriteUInt(ofs, 0);
	WriteUShort(ofs, ddsHeader.m_width);
	WriteUShort(ofs, ddsHeader.m_height);
	WriteUByte(ofs, ddsHeader.m_depth);
	WriteUByte(ofs, ddsHeader.m_mipCount);
	WriteUShort(ofs, 3);

	ofs.write(textureData, (fileSize - 0x80));

	delete [] textureData;
}

void WritePCTarga(const char* resultFileName, std::ifstream& ifs)
{
	int fileSize = 0;
	ifs.seekg(0, SEEK_END);
	fileSize = ifs.tellg();
	ifs.seekg(0, SEEK_SET);

	unsigned int magic;
	ifs.read(reinterpret_cast<char*>(&magic), sizeof(unsigned int));

	if (magic != 0x00020000)
	{
		std::cout << "Error: Targa magic mis-match!" << std::endl;
		ifs.close();
		return;
	}

	ifs.seekg(0xC, SEEK_SET);

	unsigned short width;
	unsigned short height;

	ifs.read(reinterpret_cast<char*>(&width), sizeof(unsigned short));
	ifs.read(reinterpret_cast<char*>(&height), sizeof(unsigned short));

	char* textureData = new char[width * height * 4];
	ifs.seekg(0x12, SEEK_SET);
	ifs.read(textureData, width * height * 4);
	ifs.close();

	char nameBuff[128];
	memset(nameBuff, 0, 128);
	sprintf_s(nameBuff, "%s%s", resultFileName, ".pcd");

	std::ofstream ofs(nameBuff, std::ios::binary);

	//
	std::string path(resultFileName);
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
	WriteUInt(ofs, ((fileSize - 0x80) + 0x18));
	WriteUInt(ofs, (TEXTURE_SECTION_TYPE));
	WriteUInt(ofs, 0);
	WriteUInt(ofs, hash);
	WriteUInt(ofs, 0xFFFFFFFF);

	WriteUInt(ofs, PCD_MAGIC);
	WriteUInt(ofs, 0x15);
	WriteUInt(ofs, (fileSize - 0x80));
	WriteUInt(ofs, 0);
	WriteUShort(ofs, width);
	WriteUShort(ofs, height);
	WriteUByte(ofs, 32);
	WriteUByte(ofs, 0);
	WriteUShort(ofs, 0);

	ofs.write(textureData, width * height * 4);

	delete [] textureData;
}
