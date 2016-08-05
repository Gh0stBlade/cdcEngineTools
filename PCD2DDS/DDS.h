#ifndef DDS_H
#define DDS_H

/*enum DDSFormat
{
	DXT1 = 0x31545844,
	DXT3 = 0x33545844,
	DTX5 = 0x35545844
};*/

#pragma pack(push, 1)
struct DDSHeader
{
	unsigned int m_magic;
	unsigned int m_size;
	unsigned int m_flags;
	unsigned int m_height;

	unsigned int m_width;
	unsigned int m_pitchOrLinearSize;
	unsigned int m_dummy;
	unsigned int m_depth;
	unsigned int m_mipCount;

	unsigned int m_reserved[12];
	unsigned int m_format;
};

#pragma pack(pop)

void ConvertDDSToPCD(const char* filePath);

#endif