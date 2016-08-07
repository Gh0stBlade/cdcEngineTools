#ifndef RIFF_H
#define RIFF_H

#define SECTION_MAGIC (0x54434553)
#define RIFF_MAGIC    (0x46464952)
#define WAVE_SECTION_TYPE (6)

void ConvertWAVEToWAV(const char* filePath);
void ConvertWAVToWAVE(const char* filePath);

#endif
