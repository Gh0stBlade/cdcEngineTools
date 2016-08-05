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

#include "CDRM.h"
#include "File.h"

#define ZLIB_WINAPI
#define CHUNK 32768
#include "..\External\zlib\zlib.h"

#include <iostream>

CDRM::CDRM()
{
	this->m_magic = 0;
	this->m_numCompressedBlocks = 0;
	this->m_entries.clear();
}

void CDRM::Compress(const char* filePath, unsigned int compressionMode)
{
	//Open DRM file stream as binary
	std::ifstream ifs(filePath, std::ios::binary);

	//Skip to end of file to get the size
	ifs.seekg(0, std::ios::end);
	unsigned int fileSize = (unsigned int)ifs.tellg();
	ifs.seekg(0, std::ios::beg);

	//Calculate number of compressed blocks that can be made from this file
	unsigned int numCompressedBlocks = (fileSize / CDRM_MAX_BLOCK_SIZE);

	if ((fileSize % CDRM_MAX_BLOCK_SIZE)) numCompressedBlocks++;

#if _DEBUG
	std::cout << "Number of Compressed Blocks: " << std::to_string(numCompressedBlocks) << std::endl;
#endif

	//Initialise output file stream
	std::ofstream ofs("temp.bin", std::ios::binary);

	//Seek to file data start
	ofs.seekp(0x10 + (numCompressedBlocks * 0x8), std::ios::beg);

	//Skip alignment
	ofs.seekp((((unsigned int)ofs.tellp() + CDRM_FILE_ALIGNMENT) & ~CDRM_FILE_ALIGNMENT));

	//Read each block and compress it
	for (int i = 0; i != numCompressedBlocks; i++)
	{
		std::cout << "Compressing Section: " << "[ " << (i + 1) << " / " << numCompressedBlocks << " ]" << std::endl;

		unsigned int uncompressedSize = 0;
		unsigned int compressedSize = 0;

		if ((i + 1) == numCompressedBlocks)//Final block
		{
			uncompressedSize = ((fileSize % CDRM_MAX_BLOCK_SIZE) << 8) ^ compressionMode;
		}
		else
		{
			uncompressedSize = (CDRM_MAX_BLOCK_SIZE << 8) ^ compressionMode;
		}

		char* szUncompressedData = new char[uncompressedSize >> 8];
		std::string strCompressedData;

		//Read uncompresssed data
		ifs.read(szUncompressedData, (uncompressedSize >> 8));
		
		//Compress the uncompressed data
		if (compressionMode == CDRMFlags::COMPRESSED)
		{
			//Compress the data
			CompressData(szUncompressedData, (uncompressedSize >> 8), strCompressedData);

			//Save compressed size
			compressedSize = strCompressedData.size();

			//Write the compressed data to temporary file!
			ofs.write(strCompressedData.c_str(), compressedSize);
		}
		else
		{
			//Save compressed size
			compressedSize = (uncompressedSize >> 8);

			//Write the uncompressed data to temporary file!
			ofs.write(szUncompressedData, compressedSize);
		}

		//Skip alignment
		ofs.seekp((((unsigned int)ofs.tellp() + CDRM_FILE_ALIGNMENT) & ~CDRM_FILE_ALIGNMENT));

		//Write entry info
		std::streamoff pos = ofs.tellp();
		ofs.seekp(0x10 + (i * 0x8), SEEK_SET);
		WriteUInt(ofs, uncompressedSize);
		WriteUInt(ofs, compressedSize);
		ofs.seekp(pos, SEEK_SET);

		//Delete allocated memory
		delete[] szUncompressedData;
	}

	//Seek to file data start
	ofs.seekp(0x0, std::ios::beg);

	//Write "CDRM" to header
	WriteUInt(ofs, CDRM_MAGIC);

	//Skip null 0x4
	ofs.seekp(0x4, std::ios::cur);

	//Write numCompressedBlocks to header
	WriteUInt(ofs, numCompressedBlocks);

	//Skip null 0x4
	ofs.seekp(0x4, std::ios::cur);

	//Close our streams
	ofs.flush();
	ofs.close();
	ifs.close();

	//Delete the original file
	remove(filePath);

	//Rename temporary file to our original file
	rename("temp.bin", filePath);

	//Print success
	std::cout << "Successfully Compressed: " << "[ " << (numCompressedBlocks) << " ] " << " section(s)!" << std::endl;
}

void CDRM::Decompress(const char* filePath)
{
	//Initialise ifstream for reading in binary mode
	std::ifstream ifs(filePath, std::ios::binary);

	//If it's not good to go
	if (!ifs.good())
	{
		std::cout << "Fatal Error: Failed to open file at path: " << filePath << std::endl;
		ifs.close();
		return;
	}

	//Read our CDRM header into cCDRM
	this->m_magic = ReadUInt(ifs);
	ifs.seekg(0x4, SEEK_CUR);
	if (this->m_magic != CDRM_MAGIC)
	{
		std::cout << "Fatal Error: Magic mis-match! expected: " << std::hex << CDRM_MAGIC << " got: " << std::hex << this->m_magic << std::endl;
		ifs.close();
		return;
	}

	//If too many compressed blocks error and return
	this->m_numCompressedBlocks = ReadUInt(ifs);
	ifs.seekg(0x4, SEEK_CUR);
	if (this->m_numCompressedBlocks > CDRM_MAX_COMPRESSED_BLOCKS)
	{
		std::cout << "Fatal Error: Number of Compressed Blocks: " << this->m_numCompressedBlocks << " exceeded the limit of: " << CDRM_MAX_COMPRESSED_BLOCKS << "!" << std::endl;
		ifs.close();
		return;
	}

	//If not enough compressed blocks
	if (this->m_numCompressedBlocks <= 0)
	{
		std::cout << "Fatal Error: Number of Compressed Blocks <= 0 !" << std::endl;
		ifs.close();
		return;
	}

	//Read entry info
	for (int i = 0; i != this->m_numCompressedBlocks; i++)
	{
		this->m_entries.emplace_back();
		this->m_entries[i].m_uncompressedSize = ReadUInt(ifs);
		this->m_entries[i].m_compressedSize = ReadUInt(ifs);
	}

	//Skip according to alignment
	ifs.seekg((((unsigned int)ifs.tellg() + CDRM_FILE_ALIGNMENT) & ~CDRM_FILE_ALIGNMENT));

	//Decompress each block and append to file
	for (int i = 0; i != this->m_numCompressedBlocks; i++)
	{
		//Print out decompression process info
		std::cout << "Decompressing Section: " << "[ " << (i + 1) << " / " << this->m_numCompressedBlocks << " ]" << std::endl;

		//Declare our uncompressed/compressed buffers where our data is/going to be stored.
		char* compressedData = new char[this->m_entries[i].m_compressedSize];
		char* uncompressedData = new char[this->m_entries[i].m_uncompressedSize >> 8];

#if DEBUG
		std::cout << "Start Offset: " << ifs.tellg() << " i:" << i << std::endl;
#endif
		//Read the compressed Zlib data into compressedData
		ifs.read(compressedData, this->m_entries[i].m_compressedSize);

#if DEBUG
		std::cout << "End Offset: " << ifs.tellg() << " i:" << i << std::endl;
#endif
		//Skip according to alignment
		ifs.seekg((((unsigned int)ifs.tellg() + CDRM_FILE_ALIGNMENT) & ~CDRM_FILE_ALIGNMENT));

		//If data is compressed, decompress the compressed data otherwise do nothing!
		if ((this->m_entries[i].m_uncompressedSize & 0xFF) == CDRMFlags::COMPRESSED)
		{
			DecompressData(compressedData, this->m_entries[i].m_compressedSize, uncompressedData, (this->m_entries[i].m_uncompressedSize >> 8));

			//Write decompressed blocks to a temporary file
			std::ofstream ofs("temp.bin", std::ios::binary | std::ios::app);

			//If not good to go
			if (!ofs.good())
			{
				std::cout << "Fatal Error: Unknown error occured whilst initialising ofstream!" << std::endl;
				ofs.close();
				ifs.close();
				return;
			}

			//Skip to EOF and write uncompressed data
			ofs.seekp(0, std::ios::end);
			ofs.write(uncompressedData, (this->m_entries[i].m_uncompressedSize >> 8));

			//Flush and close ofstream
			ofs.flush();
			ofs.close();
		}
		else if ((this->m_entries[i].m_uncompressedSize & 0xFF) == CDRMFlags::UNCOMPRESSED)
		{
			//Write uncompressed blocks to a temporary file
			std::ofstream ofs("temp.bin", (std::ios::binary | std::ios::app));

			//If not good to go
			if (!ofs.good())
			{
				std::cout << "Fatal Error: Unknown error occured whilst initialising ofstream!" << std::endl;
				ifs.close();
				ifs.close();
				return;
			}

			//Skip to EOF and write uncompressed data
			ofs.seekp(0, std::ios::end);
			ofs.write(compressedData, this->m_entries[i].m_compressedSize);

			//Flush and close ofstream
			ofs.flush();
			ofs.close();
		}
		else
		{
			std::cout << "Fatal Error: Unknown Compression flag: " << std::to_string((this->m_entries[i].m_uncompressedSize & 0xFF)) << std::endl;
			system("Pause");
		}

		//Delete the compressed/decompressed data from the memory
		delete[] uncompressedData;
		delete[] compressedData;
	}

	//Close file cdrm ifstream
	ifs.close();

	//Delete the original file
	remove(filePath);

	//Rename temporary file to our original file
	rename("temp.bin", filePath);

	//Print end success message
	std::cout << "Successfully Decompressed: " << "[ " << (this->m_numCompressedBlocks) << " ] " << " section(s)!" << std::endl;
}

void DecompressData(char* &compressedData, unsigned int compressedSize, char* &uncompressedData, unsigned int uncompressedSize)
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
	strm.avail_in = compressedSize;//Compressed Size
	strm.next_in = (Bytef *)compressedData;//Compressed data char*
	strm.avail_out = uncompressedSize;//UnCompressed Size
	strm.next_out = (Bytef *)uncompressedData; //Uncompressed data char*

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

CDRM::~CDRM()
{
	if (this->m_entries.size() > 0 && this->m_numCompressedBlocks > 0)
	{
		this->m_entries.clear();
	}

	this->m_magic = 0;
	this->m_numCompressedBlocks = 0;
}