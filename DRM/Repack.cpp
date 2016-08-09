#include "Repack.h"

#include "DRM.h"
#include "File.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

void RepackSections(const char* sectionListPath, const char* basePath)
{
	
	std::string sectionFileName;
	int numSections = 0;
	std::ofstream ofs("Repacked.drm", std::ios::binary);

	WriteUInt(ofs, DRM_VERSION);
	WriteUInt(ofs, 0);//Placeholder

	//Read and write DRM headers
	std::ifstream sectionListFile(sectionListPath);
	while (std::getline(sectionListFile, sectionFileName))
	{
		numSections++;

		std::stringstream sectionPath;
		sectionPath << basePath << "\\" << sectionFileName;

		std::ifstream ifs(sectionPath.str(), std::ios::binary);
		int sectHdr = ReadUInt(ifs);//Unused

		//
		std::string path(sectionPath.str());
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

		WriteUInt(ofs, ReadUInt(ifs));
		WriteUByte(ofs, ReadUByte(ifs));
		WriteUByte(ofs, ReadUByte(ifs));
		WriteUShort(ofs, ReadUShort(ifs));
		WriteUInt(ofs, ReadUInt(ifs));
		unsigned int sectionHash = ReadUInt(ifs);
		if (sectionHash != hash)
		{
			std::cout << "Warning: Detected hash mis-match!" << std::endl;
			sectionHash = hash;
		}
		WriteUInt(ofs, sectionHash);
		
		WriteUInt(ofs, ReadUInt(ifs));
		ifs.close();
	}
	sectionListFile.close();

	//Read and write section data
	std::ifstream sectionListFile2(sectionListPath);
	while (std::getline(sectionListFile2, sectionFileName))
	{
		std::stringstream sectionPath;
		sectionPath << basePath << "\\" << sectionFileName;

		std::ifstream ifs(sectionPath.str(), std::ios::binary);
		
		//Get section data size
		ifs.seekg(0x0, SEEK_END);
		int dataSize = (ifs.tellg());
		dataSize -= 0x18;
		ifs.seekg(0x18, SEEK_SET);

		char* sectionData = new char[dataSize];
		ifs.read(sectionData, dataSize);
		ofs.write(sectionData, dataSize);
		delete[] sectionData;
		
		ifs.close();
	}
	sectionListFile2.close();


	//Write num files
	ofs.seekp(0x4, SEEK_SET);
	WriteUInt(ofs, numSections);

	ofs.flush();
	ofs.close();

}