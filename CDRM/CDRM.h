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

//Definitions
#define  CDRM_MAGIC (0x4D524443)
#define  CDRM_MAX_COMPRESSED_BLOCKS (16777215)

//Platform specific
#if PC || XENON || PS3
#define CDRM_MAX_BLOCK_SIZE (0x40000)
#define CDRM_FILE_ALIGNMENT (0xF)
#elif WII
#define CDRM_MAX_BLOCK_SIZE (0x20000)
#define CDRM_FILE_ALIGNMENT (0x1F)
#else
#error "Unsupported Platform!"
#endif

//Classes
class cCDRM
{
	class Entry;

public:
	void Decompress(char* szFilePath);
	void Destroy();

private:
	char* szFilePath;

#if TR8///@FIXME remove?
	unsigned int uiMagic;
	unsigned int uiNumCompressedBlocks;
	Entry *pEntries = NULL;
#endif
	
};

class cCDRM::Entry
{
public:
	enum Flags;
	unsigned int uiCompressedSize;
	unsigned int uiUnCompressedSize;
	void DecompressEntry(char* &szCompressedData, char* &szUnCompressedData);
};

//Enums
enum cCDRM::Entry::Flags
{
	UNKNOWN = 0,
	UNCOMPRESSED = 1,
	COMPRESSED = 2
};

#endif

