/*
	[CDRM] Tomb Raider: Legend/Anniversary/Underworld CDRM Decompressor
	Copyright (C) Gh0stBlade 2015 - gh0stblade@live[dot]co.uk

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef CDRM_H
#define CDRM_H

//Includes
#include <stdio.h>
#include <sstream>
#include <vector>

//Constants
const unsigned int CDRM_MAGIC = 0x4D524443;///@TODO Endian big!
const unsigned int CDRM_MAX_COMPRESSED_BLOCKS = 16777215;

//Platform specific constants
#if (PC || XENON || PS3)
	const unsigned int CDRM_MAX_BLOCK_SIZE = 262144;
	const unsigned short CDRM_FILE_ALIGNMENT = 15;
#elif (WII)
	const unsigned int CDRM_MAX_BLOCK_SIZE = 131072;
	const unsigned short CDRM_FILE_ALIGNMENT = 31;
#else
	#error "Unsupported Platform!"
#endif

//Structs
struct CDRMEntry
{
	unsigned int m_compressedSize;
	unsigned int m_uncompressedSize;
};

//Classes
class CDRM
{
public:
	CDRM();
	~CDRM();

	void Decompress(const char* filePath);
	void Compress(const char* filePath, unsigned int compressionMode);

private:
	unsigned int m_magic;
	unsigned int m_numCompressedBlocks;
	std::vector<CDRMEntry> m_entries;
};


//Enums
enum CDRMFlags
{
	NONE = 0,
	UNCOMPRESSED = 1,
	COMPRESSED = 2
};

void CompressData(char* szUncompressedData, unsigned int uiUncompressedSize, std::string &strOutData);
void DecompressData(char* &szCompressedData, unsigned int compressedSize, char* &szUnCompressedData, unsigned int uncompressedSize);

#endif

