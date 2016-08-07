#include "RIFF.h"
#include "File.h"

#include <iostream>
#include <fstream>

void ConvertWAVEToWAV(const char* filePath)
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
	if (sectionType != WAVE_SECTION_TYPE)
	{
		std::cout << "Error: invalid section type!" << std::endl;
		ifs.close();
		return;
	}

	//Now we've validated this is a valid section let's convert the data to wav
	ifs.seekg(24, SEEK_SET);
	
	unsigned int waveFrequency = ReadUInt(ifs);

	ifs.seekg(8, SEEK_CUR);

	unsigned int waveUnk00 = ReadUInt(ifs);

	char* waveData = new char[sectionSize - 16];
	ifs.read((char*)waveData, (sectionSize - 16));

	ifs.close();

	//Write output wave file
	char nameBuff[128];
	memset(nameBuff, 0, 128);
	sprintf_s(nameBuff, "%s%s", filePath, ".wav");///@FIXME filename needs extracting this is dangerous!

	std::ofstream ofs(nameBuff, std::ios::binary);
	//"RIFF"
	WriteUInt(ofs, RIFF_MAGIC);
	//Size
	WriteUInt(ofs, ((sectionSize-16) + 44));
	//"WAVE"
	WriteUInt(ofs, 0x45564157);
	//"fmt "
	WriteUInt(ofs, 0x20746D66);
	//Sub chunk
	WriteUInt(ofs, 0x14);
	//Audio format
	WriteUShort(ofs, 0x11);
	//Num channels
	WriteUShort(ofs, 1);
	//Sample rate
	WriteUInt(ofs, waveFrequency);
	//Byte rate
	WriteUInt(ofs, 0x4DE2);
	//Block align
	WriteUShort(ofs, 36);
	//Bits per sample
	WriteUShort(ofs, 4);
	//?
	WriteUShort(ofs, 2);
	//?
	WriteUShort(ofs, 65);
	//"DATA"
	WriteUInt(ofs, 0x61746164);
	//?
	WriteUInt(ofs, 0xD530);
	//?
	WriteUInt(ofs, waveUnk00);

	ofs.write(waveData, (sectionSize - 16));
	delete[] waveData;
	ofs.close();

}

void ConvertWAVToWAVE(const char* filePath)
{
	std::ifstream ifs(filePath, std::ios::binary);

	//If there was a failure to open the file we must exit
	if (!ifs.good())
	{
		std::cout << "Error: failed to open the file!" << std::endl;
		ifs.close();
		return;
	}

	unsigned int riffMagic = ReadUInt(ifs);
	if (riffMagic != RIFF_MAGIC)
	{
		std::cout << "Error: RIFF magic mis-match!" << std::endl;
		ifs.close();
		return;
	}

	unsigned int riffSize = ReadUInt(ifs);
	if (riffMagic <= 0)
	{
		std::cout << "Error: RIFF size <= 0!" << std::endl;
		ifs.close();
		return;
	}

	ifs.seekg(12, SEEK_CUR);

	unsigned short riffFormat = ReadUShort(ifs);
	if (riffFormat != 0x11)
	{
		std::cout << "Error: unsupported wave format! Got:" << riffFormat << "Expected: 18" << std::endl;
		ifs.close();
		return;
	}

	ifs.seekg(2, SEEK_CUR);

	unsigned int riffSampleRate = ReadUInt(ifs);

	ifs.seekg(20, SEEK_CUR);

	unsigned int riffUnk00 = ReadUInt(ifs);

	char* waveData = new char[riffSize - 44];
	ifs.read(waveData, (riffSize - 4));

	ifs.close();

	//Write section header
	char nameBuff[128];
	memset(nameBuff, 0, 128);
	sprintf_s(nameBuff, "%s%s", filePath, ".wave");

	std::ofstream ofs(nameBuff, std::ios::binary);

	if (!ofs.good())
	{
		std::cout << "Error: Failed to open output file for writing!" << std::endl;
		ofs.close();
		return;
	}

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

	//"SECT"
	WriteUInt(ofs, 0x54434553);
	//File size of section data minus header
	WriteUInt(ofs, ((riffSize-44)+16));
	WriteUByte(ofs, WAVE_SECTION_TYPE);
	ofs.seekp(3, SEEK_CUR);//Unsupported
	//Header size (nothing since this asset is not relocated)
	WriteUInt(ofs, 0);
	//Unique hash of section stored in filename
	WriteUInt(ofs, hash);
	//Lang/mask
	WriteUInt(ofs, 0xFFFFFFFF);
	
	//Wave section data
	WriteUInt(ofs, riffSampleRate);
	ofs.seekp(8, SEEK_CUR);
	WriteUInt(ofs, riffUnk00);
	ofs.write(waveData, (riffSize - 44));

	ofs.flush();
	ofs.close();
}