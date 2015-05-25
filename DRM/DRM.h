#ifndef DRM_H
#define DRM_H

//Includes

//Classes
class cDRM
{
	class Section;

public:
	void LoadFromFile(char* szFilePath);

private:
	unsigned int uiVersion;

#if TR8
	unsigned int uiNameSize;
	unsigned int uiPaddingSize;
	unsigned int uiUnk00;
	unsigned int uiUnk02;
#endif

	unsigned int uiNumSections;

	Section *pSections;
};

class cDRM::Section
{
	enum Types;

	unsigned int uiSize;
	unsigned char ucType;
	unsigned char ucUnk00;
	unsigned short usUnk01;
	unsigned int uiHeaderSize;
	unsigned int uiHash;
	unsigned int uiLang;
};


#endif

//Enums
enum cDRM::Section::Types
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
	UNK00 = 0,
	UNK01 = 1,
	ANIMATION = 2,
	UNK03 = 3,
	UNK04 = 4,
	TEXTURE = 5,
	AUDIO = 6,
	DTP = 7,
	SCRIPT = 8,
	SHADER = 9,
	MATERIAL = 10,
	OBJ = 11,
	MESH = 12,
	UNK13 = 13
#endif

};