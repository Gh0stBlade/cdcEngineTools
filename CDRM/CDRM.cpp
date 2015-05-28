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

#define ZLIB_WINAPI
#define CHUNK 32768

#include "CDRM.h"
#include "File.h"
#include "FileExtensions.h"
#include "..\External\zlib\zlib.h"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

void cCDRM::Decompress(char* szFilePath)
{
	//Store CDRM input path
	this->szFilePath = szFilePath;

	//Initialise ifstream for reading in binary mode
	std::ifstream ifs(this->szFilePath, std::ios::binary);

	//If it's not good to go
	if (!ifs.good())
	{
		std::cout << "Fatal Error: Failed to open file at path: " << this->szFilePath << std::endl;
		return;
	}

	//Read our CDRM header into cCDRM
	this->uiMagic = ReadUInt(ifs);
	ifs.seekg(0x4, SEEK_CUR);
	if (this->uiMagic != CDRM_MAGIC)
	{
		std::cout << "Fatal Error: Magic mis-match! expected: " << std::hex << CDRM_MAGIC << " got: " << std::hex << this->uiMagic << std::endl;
		return;
	}

	//If too many compressed blocks error and return
	this->uiNumCompressedBlocks = ReadUInt(ifs);
	ifs.seekg(0x4, SEEK_CUR);
	if (this->uiNumCompressedBlocks > CDRM_MAX_COMPRESSED_BLOCKS)
	{
		std::cout << "Fatal Error: Number of Compressed Blocks: " << this->uiNumCompressedBlocks << " exceeded the limit of: " << CDRM_MAX_COMPRESSED_BLOCKS << "!" << std::endl;
		return;
	}

	//If not enough compressed blocks
	if (this->uiNumCompressedBlocks <= 0)
	{
		std::cout << "Fatal Error: Number of Compressed Blocks <= 0 !" << std::endl;
		return;
	}

	//Read entry info
	this->pEntries = new Entry[this->uiNumCompressedBlocks];
	for (int i = 0; i != this->uiNumCompressedBlocks; i++)
	{
		this->pEntries[i].uiUnCompressedSize = ReadUInt(ifs);
		this->pEntries[i].uiCompressedSize = ReadUInt(ifs);
	}

	//Skip according to alignment
	ifs.seekg((((unsigned int)ifs.tellg() + CDRM_FILE_ALIGNMENT) & ~CDRM_FILE_ALIGNMENT));

	//Decompress each block and append to file
	for (int i = 0; i != this->uiNumCompressedBlocks; i++)
	{
		//Print out decompression process info
		std::cout << "Decompressing Section: " << "[ " << (i + 1) << " / " << this->uiNumCompressedBlocks << " ]" << std::endl;

		//Declare our uncompressed/compressed buffers where our data is/going to be stored.
		char* szCompressedData = new char[this->pEntries[i].uiCompressedSize];
		char* szUncompressedData = new char[this->pEntries[i].uiUnCompressedSize >> 8];

#if _DEBUG
		std::cout << "Start Offset: " << ifs.tellg() << " i:" << i << std::endl;
#endif
		//Read the compressed Zlib data into compressedData
		ifs.read(szCompressedData, this->pEntries[i].uiCompressedSize);

#if _DEBUG
		std::cout << "End Offset: " << ifs.tellg() << " i:" << i << std::endl;
#endif
		//Skip according to alignment
		ifs.seekg((((unsigned int)ifs.tellg() + CDRM_FILE_ALIGNMENT) & ~CDRM_FILE_ALIGNMENT));

		//If data is compressed, decompress the compressed data otherwise do nothing!
		if ((this->pEntries[i].uiUnCompressedSize & 0xFF) == cCDRM::Entry::COMPRESSED)
		{
			this->pEntries[i].DecompressEntry(szCompressedData, szUncompressedData);
			//Write decompressed blocks to a temporary file
			std::ofstream ofs("temp.bin", std::ios::binary | std::ios::app);
			ofs.seekp(0, std::ios::end);
			ofs.write(szUncompressedData, (this->pEntries[i].uiUnCompressedSize >> 8));
			ofs.flush();
			ofs.close();
		}
		else if ((this->pEntries[i].uiUnCompressedSize & 0xFF) == cCDRM::Entry::COMPRESSED)
		{
			//Write uncompressed blocks to a temporary file
			std::ofstream ofs("temp.bin", std::ios::binary | std::ios::app);
			ofs.seekp(0, std::ios::end);
			ofs.write(szCompressedData, (this->pEntries[i].uiCompressedSize));
			ofs.flush();
			ofs.close();
		}
		else
		{
			std::cout << "Fatal Error: Unknown Compression flag: " << std::to_string((this->pEntries[i].uiUnCompressedSize & 0xFF)) << std::endl;
			system("Pause");
		}

		//Delete the compressed/decompressed data from the memory
		delete[] szUncompressedData;
		delete[] szCompressedData;
	}

	//Close file cdrm ifstream
	ifs.close();

	//Delete the original file
	remove(this->szFilePath);

	//Rename temporary file to our original file
	rename("temp.bin", this->szFilePath);

	//Print end success message
	std::cout << "Successfully Decompressed: " << "[ " << (this->uiNumCompressedBlocks) << " ] " << " section(s)!" << std::endl;
}

void cCDRM::Entry::DecompressEntry(char* &szCompressedData, char* &szUnCompressedData)
{
	//Declare zlib inflate stream as strm and our place to return errors, ret
	z_stream strm;
	int ret;
	bool done = false;

	//Allocate inflate state
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	ret = inflateInit(&strm);

	//If inflate didn't successfully initialise
	if (ret != Z_OK)///@FIMEX Z_STREAM_END?
	{
		std::cout << "Fatal Error: Failed to initialise inflate stream!" << std::endl;
		system("Pause");
		return;//@FIXME? Ret?
	}

	//Tell Zlib where to find the data and their sizes
	strm.avail_in = uiCompressedSize;//Compressed Size
	strm.next_in = (Bytef *)szCompressedData;//Compressed data char*
	strm.avail_out = uiUnCompressedSize;//UnCompressed Size
	strm.next_out = (Bytef *)szUnCompressedData; //Uncompressed data char*

	//Inflate the stream itself
	ret = inflate(&strm, Z_NO_FLUSH);

	//Temp move to own function?
	switch (ret)
	{
	case Z_VERSION_ERROR:
		std::cout << "Z_VERSION_ERROR FATAL!" << std::endl;
		system("pause");
		break;
	case Z_STREAM_ERROR:
		std::cout << "Z_STREAM_ERROR FATAL!" << std::endl;
		system("pause");
		break;
	}

	//End the inflation
	inflateEnd(&strm);
}

void cCDRM::Destroy()
{
	if (this->pEntries != NULL && this->uiNumCompressedBlocks > 0)
		delete[] this->pEntries;
}