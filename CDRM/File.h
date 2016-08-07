#ifndef FILE_H
#define FILE_H

#include "PlatformSetup.h"

#include <fstream>
#include <iostream>

unsigned int ReverseUInt(unsigned int uiInput);
int ReverseInt(int iInput);
unsigned short ReverseUShort(unsigned short usInput);
short ReverseShort(short sInput);

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

unsigned int GetFileType(const char* szFilePath);

#endif