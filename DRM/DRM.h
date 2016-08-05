/*
	[DRM] Tomb Raider: Legend/Anniversary/Underworld DRM Unpacker
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

#ifndef DRM_H
#define DRM_H

//Includes
#include <vector>

//Constants
const unsigned int DRM_MAX_SECTIONS = 16777215;

#if (TR7 || TRAE)
	#define DRM_VERSION (14)
#elif TR8
	#define DRM_VERSION (19)
#else
	#error "Unsupported Game!"
#endif

struct Section
{
	unsigned int uiSize;
	unsigned char ucType;
	unsigned char ucUnk00;
	unsigned short usUnk01;
	unsigned int uiHeaderSize;
	unsigned int uiHash;
	unsigned int uiLang;
};

//Classes
class cDRM
{

public:
	void ExtractSections(char* szFilePath);
	void Destroy();

private:
	char* szFilePath;
	unsigned int m_version;

#if TR8
	unsigned int m_nameSize;
	unsigned int m_paddingSize;
	unsigned int m_unk00;
	unsigned int m_unk01;
#endif

	unsigned int m_numSections;

	std::vector<Section> m_sections;
};

//Enums
enum SectionType
{
#if TR7
	GENERAL = 0,
	EMPTY = 1,
	ANIMATION = 2,
	PUSHBUFFER_WC = 3,
	PUSHBUFFER = 4,
	TEXTURE = 5,
	WAVE = 6,
	DTPDATA = 7,
	SCRIPT = 8,
	SHADERLIB = 9,
	NUM_SECTION_TYPES = 10

#elif TR8
	GENERAL = 0,
	EMPTY = 1,
	ANIMATION = 2,
	PUSHBUFFER_WC = 3,
	PUSHBUFFER = 4,
	TEXTURE = 5,
	WAVE = 6,
	DTPDATA = 7,
	SCRIPT = 8,
	SHADERLIB = 9,
	MATERIAL = 10,
	OBJ = 11,
	MESH = 12,
	UNK13 = 13
#endif

};

#endif