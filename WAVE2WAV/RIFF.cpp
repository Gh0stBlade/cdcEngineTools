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

	if (!ifs.good())
	{
		ifs.close();
		return;
	}
}