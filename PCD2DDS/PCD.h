#ifndef PCD_H
#define PCD_H

#define SECTION_MAGIC (0x54434553)
#define PCD_MAGIC     (0x39444350)
#define DDS_MAGIC     (0x20534444)
#define TEXTURE_SECTION_TYPE (5)

enum PCDFormat
{
	DXT1 = 0x31545844,
	DXT3 = 0x33545844,
	DTX5 = 0x35545844
};

enum PCDFlags
{
	FILTER_POINT = 0x0,
	FILTER_BILINEAR = 0x1,
	FILTER_TRILINEAR = 0x2,
	FILTER_ANISOTROPIC_1X = 0x3,
	FILTER_ANISOTROPIC_2X = 0x4,
	FILTER_ANISOTROPIC_4X = 0x6,
	FILTER_ANISOTROPIC_8X = 0xa,
	FILTER_ANISOTROPIC_16X = 0x12,
	FILTER_BEST = 0x100,
	FILTER_DEFAULT = 0x12,
	FILTER_INVALID = 0x200,
};

#pragma pack(push, 1)
struct PCTextureDataHeader
{
	unsigned int m_magic;
	enum PCDFMT m_format;
	unsigned int m_textureDataSize;
	unsigned int m_paletteDataSize;
	unsigned short m_width;
	unsigned short m_height;
	unsigned char m_depth;
	unsigned char m_numMipMaps;
	unsigned short m_flags;
};

#pragma pack(pop)

void ConvertPCDToDDS(const char* filePath);

#endif