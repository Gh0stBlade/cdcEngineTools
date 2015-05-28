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

void cCDRM::Compress(const char* szFilePath, unsigned int uiCompressionMode)
{
	//Open DRM file stream as binary
	std::ifstream ifs(szFilePath, std::ios::binary);

	//Skip to end of file to get the size
	ifs.seekg(0, std::ios::end);
	unsigned int uiFileSize = (unsigned int)ifs.tellg();
	ifs.seekg(0, std::ios::beg);

	//Calculate number of compressed blocks that can be made from this file
	unsigned int uiNumCompressedBlocks = (uiFileSize / CDRM_MAX_BLOCK_SIZE) + 1;

#if _DEBUG
	std::cout << "Number of Compressed Blocks: " << std::to_string(uiNumCompressedBlocks) << std::endl;
#endif

	//Declare variables to store the compressed/uncompressed size info
	unsigned int* uiUnCompressedSize = new unsigned int[uiNumCompressedBlocks];
	unsigned int* uiCompressedSize = new unsigned int[uiNumCompressedBlocks];

	//Initialise output file stream
	std::ofstream ofs("temp.bin", std::ios::binary);

	//Seek to file data start
	ofs.seekp(0x10 + (uiNumCompressedBlocks * 0x8), std::ios::beg);

	//Skip alignment
	ofs.seekp((((unsigned int)ofs.tellp() + CDRM_FILE_ALIGNMENT) & ~CDRM_FILE_ALIGNMENT));

	//Read each block and compress it
	for (int i = 0; i != uiNumCompressedBlocks; i++)
	{
		std::cout << "Compressing Section: " << "[ " << (i + 1) << " / " << uiNumCompressedBlocks << " ]" << std::endl;

		if ((i + 1) == uiNumCompressedBlocks)
		{
			uiUnCompressedSize[i] = (((uiFileSize)-(i * CDRM_MAX_BLOCK_SIZE)) << 8) ^ uiCompressionMode;
		}
		else
		{
			uiUnCompressedSize[i] = (CDRM_MAX_BLOCK_SIZE << 8) ^ uiCompressionMode;
		}

		char* szUncompressedData = new char[(uiUnCompressedSize[i]) >> 8];
		std::string strCompressedData;

		//Read uncompresssed data
		ifs.read(szUncompressedData, (uiUnCompressedSize[i] >> 8));
		
		//Compress the uncompressed data
		if (uiCompressionMode == cCDRM::Entry::COMPRESSED)
		{
			//Compress the data
			CompressData(szUncompressedData, (uiUnCompressedSize[i] >> 8), strCompressedData);

			//Save compressed size
			uiCompressedSize[i] = strCompressedData.size();

			//Write the compressed data to temporary file!
			ofs.write(strCompressedData.c_str(), uiCompressedSize[i]);
		}
		else
		{
			//Save compressed size
			uiCompressedSize[i] = (uiUnCompressedSize[i] >> 8);

			//Write the uncompressed data to temporary file!
			ofs.write(szUncompressedData, uiCompressedSize[i]);
		}

		//Skip alignment
		ofs.seekp((((unsigned int)ofs.tellp() + CDRM_FILE_ALIGNMENT) & ~CDRM_FILE_ALIGNMENT));

		//Delete allocated memory
		delete[] szUncompressedData;
	}

	//Seek to file data start
	ofs.seekp(0x0, std::ios::beg);

	//Write "CDRM" to header
	WriteUInt(ofs, (unsigned int)CDRM_MAGIC);

	//Skip null 0x4
	ofs.seekp(0x4, std::ios::cur);

	//Write uiNumBlocks to header
	WriteUInt(ofs, uiNumCompressedBlocks);

	//Skip null 0x4
	ofs.seekp(0x4, std::ios::cur);

	for (int i = 0; i != uiNumCompressedBlocks; i++)
	{
		WriteUInt(ofs, uiUnCompressedSize[i]);
		WriteUInt(ofs, uiCompressedSize[i]);
	}

	//Close our streams
	ofs.flush();
	ofs.close();
	ifs.close();

	//Delete the original file
	remove(szFilePath);

	//Rename temporary file to our original file
	rename("temp.bin", szFilePath);

	//Print success
	std::cout << "Successfully Compressed: " << "[ " << (uiNumCompressedBlocks) << " ] " << " section(s)!" << std::endl;

	//Clear allocated memory
	delete[] uiUnCompressedSize;
	delete[] uiCompressedSize;
}

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

#if DEBUG
		std::cout << "Start Offset: " << ifs.tellg() << " i:" << i << std::endl;
#endif
		//Read the compressed Zlib data into compressedData
		ifs.read(szCompressedData, this->pEntries[i].uiCompressedSize);

#if DEBUG
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

			//If not good to go
			if (!ofs.good())
			{
				std::cout << "Fatal Error: Unknown error occured whilst initialising ofstream!" << std::endl;
				return;
			}

			//Skip to EOF and write uncompressed data
			ofs.seekp(0, std::ios::end);
			ofs.write(szUncompressedData, (this->pEntries[i].uiUnCompressedSize >> 8));

			//Flush and close ofstream
			ofs.flush();
			ofs.close();
		}
		else if ((this->pEntries[i].uiUnCompressedSize & 0xFF) == cCDRM::Entry::UNCOMPRESSED)
		{
			//Write uncompressed blocks to a temporary file
			std::ofstream ofs("temp.bin", std::ios::binary | std::ios::app);

			//If not good to go
			if (!ofs.good())
			{
				std::cout << "Fatal Error: Unknown error occured whilst initialising ofstream!" << std::endl;
				return;
			}

			//Skip to EOF and write uncompressed data
			ofs.seekp(0, std::ios::end);
			ofs.write(szCompressedData, (this->pEntries[i].uiCompressedSize));

			//Flush and close ofstream
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

void CompressData(char* szUncompressedData, unsigned int uiUncompressedSize, std::string &strOutData)
{
	std::string strBuffer;
	const size_t bufferLength = 128 * 1024;
	char szBuffer[bufferLength];

	z_stream strm;
	strm.zalloc = 0;
	strm.zfree = 0;
	strm.next_in = (Bytef*)szUncompressedData;
	strm.avail_in = uiUncompressedSize;
	strm.next_out = (Bytef*)szBuffer;
	strm.avail_out = bufferLength;

	deflateInit(&strm, Z_BEST_COMPRESSION);

	while (strm.avail_in != 0)
	{
		int res = deflate(&strm, Z_NO_FLUSH);
		if (strm.avail_out == 0)
		{
			strBuffer.insert(strBuffer.end(), szBuffer, szBuffer + bufferLength);
			strm.next_out = (Bytef*)szBuffer;
			strm.avail_out = bufferLength;
		}
	}

	int r = Z_OK;
	while (r == Z_OK)
	{
		if (strm.avail_out == 0)
		{
			strBuffer.insert(strBuffer.end(), szBuffer, szBuffer + bufferLength);
			strm.next_out = (Bytef*)szBuffer;
			strm.avail_out = bufferLength;
		}
		r = deflate(&strm, Z_FINISH);
	}

	strBuffer.insert(strBuffer.end(), szBuffer, szBuffer + bufferLength - strm.avail_out);
	deflateEnd(&strm);

	strOutData.swap(strBuffer);
}

void cCDRM::Destroy()
{
	if (this->pEntries != NULL && this->uiNumCompressedBlocks > 0)
		delete[] this->pEntries;
}