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

#include <fstream>
#include <iostream>
#include <Shlwapi.h>

#include "PlatformSetup.h"

unsigned int ReverseUInt(unsigned int uiInput)
{
	return (unsigned int)(((uiInput & 0xFFu) << 24) | ((uiInput & 0xFF00u) << 8) | ((uiInput & 0xFF0000u) >> 8) | ((uiInput & 0xFF000000u) >> 24));
}

int ReverseInt(int iInput)
{
	return (int)(((iInput & 0xFF) << 24) | ((iInput & 0xFF00u) << 8) | ((iInput & 0xFF0000u) >> 8) | ((iInput & 0xFF000000u) >> 24));
}

unsigned short ReverseUShort(unsigned short usInput)
{
	return (unsigned short)((usInput >> 8) | (usInput << 8));
}

short ReverseShort(short sInput)
{
	return (short)((sInput >> 8) | (sInput << 8));
}

char ReadByte(std::ifstream& ifs)
{
	char val;
	ifs.read((char*)&val, sizeof(char));
	return val;
}

unsigned char ReadUByte(std::ifstream& ifs)
{
	unsigned char val;
	ifs.read((char*)&val, sizeof(unsigned char));
	return val;
}

short ReadShort(std::ifstream& ifs)
{
	short val;
	ifs.read((char*)&val, sizeof(short));
#if ENDIAN_BIG
	ReverseShort(val);
#endif
	return val;
}

unsigned short ReadUShort(std::ifstream& ifs)
{
	unsigned short val;
	ifs.read((char*)&val, sizeof(unsigned short));
#if ENDIAN_BIG
	ReverseUShort(val);
#endif
	return val;
}

int ReadInt(std::ifstream& ifs)
{
	int val;
	ifs.read((char*)&val, sizeof(int));
#if ENDIAN_BIG
	ReverseInt(val);
#endif
	return val;
}

unsigned int ReadUInt(std::ifstream& ifs)
{
	unsigned int val;
	ifs.read((char*)&val, sizeof(unsigned int));
#if ENDIAN_BIG
	val = ReverseUInt(val);
#endif
	return val;
}

void WriteByte(std::ofstream& ofs, char input)
{
	char val = input;
	ofs.write(&val, sizeof(char));
}

void WriteUByte(std::ofstream& ofs, unsigned char input)
{
	unsigned char val = input;
	ofs.write((char*)&val, sizeof(unsigned char));
}

void WriteShort(std::ofstream& ofs, short input)
{
	short val = input;
#if ENDIAN_BIG
	val = ReverseShort(val);
#endif
	ofs.write((char*)&val, sizeof(short));
}

void WriteUShort(std::ofstream& ofs, unsigned short input)
{
	unsigned short val = input;
#if ENDIAN_BIG
	val = ReverseUShort(val);
#endif
	ofs.write((char*)&val, sizeof(unsigned short));
}

void WriteInt(std::ofstream& ofs, int input)
{
	int val = input;
#if ENDIAN_BIG
	val = ReverseInt(val);
#endif
	ofs.write((char*)&val, sizeof(int));
}

void WriteUInt(std::ofstream& ofs, unsigned int input)
{
	unsigned int val = input;
#if ENDIAN_BIG
	val = ReverseUInt(val);
#endif
	ofs.write((char*)&val, sizeof(unsigned int));
}

unsigned int GetFileType(const char* szFilePath)
{
	std::ifstream ifs(szFilePath, std::ios::binary);

	if (!ifs.good())
	{
		std::cout << "Fatal Error: Failed to open file!" << std::endl;
		return -1;
	}

	unsigned int uiMagic = ReadUInt(ifs);
	ifs.close();
	return uiMagic;
}