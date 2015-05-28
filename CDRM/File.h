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

#ifndef FILE_H
#define FILE_H

#include "PlatformSetup.h"

#include <fstream>
#include <iostream>

template <class t>
void Reverse(t *t);

char ReadByte(std::ifstream& ifs);
unsigned char ReadUByte(std::ifstream& ifs);
short ReadShort(std::ifstream& ifs);
unsigned short ReadUShort(std::ifstream& ifs);
int ReadInt(std::ifstream& ifs);
unsigned int ReadUInt(std::ifstream& ifs);

void WriteByte(std::ofstream& ofs, char input);
void WriteUByte(std::ofstream& ofs, unsigned char input);
void WriteShort(std::ofstream& ofs, short input);
void WriteUShort(std::ofstream& ofs, unsigned short input);
void WriteInt(std::ofstream& ofs, int input);
void WriteUInt(std::ofstream& ofs, unsigned int input);

void CreateDirectories(std::string str);

#endif