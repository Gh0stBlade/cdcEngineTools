#ifndef FILE_H
#define FILE_H

#include <fstream>
#include <iostream>

unsigned int getFileMagic(const char* filePath);

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

void CreateDirectories(std::string str);
bool IsDirectory(const char* filePath);
bool DoesFileExist(const char* filePath);

#endif