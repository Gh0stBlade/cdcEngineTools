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

#include <stdio.h>
#include <iostream>

#include "CDRM.h"
#include "Game.h"
#include "PlatformSetup.h"
#include "File.h"

void PrintToolInfo();
void PrintToolUsage();

int main(int argc, char* argv[])
{
	//Print the tool info on-screen.
	PrintToolInfo();

	//If there are args to process.
	if (argc == 2)
	{
		//Get filetype and perform specific action
		switch (GetFileType(argv[1]))
		{
		case -1://Error occured
			return 0;
		case CDRM_MAGIC: //"CDRM"
		{
			//Decompress the cdrm from input path
			CDRM mCDRM;
			mCDRM.Decompress(argv[1]);
			break;
		}
		default:
		{	//Compress the drm from the input path
			CDRM mCDRM;
			mCDRM.Compress(argv[1], 2);
			break;
		}
	}

		
	}
	else
	{
		std::cout << "Fatal Error: Insufficient amount of args!" << std::endl;
		PrintToolUsage();
	}

	return 0;
}

void PrintToolInfo()
{
#if DEBUG
	std::cout << "CDRM v1.1 (DEBUG) for " << GAME_NAME << std::endl;
#else
	std::cout << "CDRM v1.1 for " << GAME_NAME << std::endl;
#endif
	std::cout << "Platform: " << PLATFORM_FULL_NAME << " (" << PLATFORM_CONFIG_NAME << ")" << std::endl;
	std::cout << "Built: " << (__DATE__ ", " __TIME__) << std::endl;
	std::cout << "By Gh0stBlade" << std::endl;
}

void PrintToolUsage()
{
	std::cout << "CDRM.exe [input.drm]" << std::endl;
	system("pause");
}