#include <stdio.h>
#include <iostream>

#include "DRM.h"
#include "Game.h"
#include "PlatformSetup.h"
#include "File.h"
#include "Repack.h"

void PrintToolInfo();
void PrintToolUsage();

int main(int argc, char* argv[])
{
	//Print the tool info on-screen.
	PrintToolInfo();

	//If there are args to process.
	if (argc == 2)
	{
		if (IsDirectory(argv[1]))
		{
			char buff[128];
			sprintf_s(buff, "%s%s", argv[1], "\\sectionList.txt");//unsafe

			if (DoesFileExist(buff))
			{
				RepackSections(buff, argv[1]);
			}
			else
			{
				std::cout << "Warning failed to locate section list!" << std::endl;
				system("Pause");
			}
		}
		else
		{
			cDRM mDRM;
			mDRM.ExtractSections(argv[1]);
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
	std::cout << "DRM v1.2 (DEBUG) for " << GAME_NAME << std::endl;
#else
	std::cout << "DRM v1.2 for " << GAME_NAME << std::endl;
#endif
	std::cout << "Platform: " << PLATFORM_FULL_NAME << " (" << PLATFORM_CONFIG_NAME << ")" << std::endl;
	std::cout << "Built: " << (__DATE__ ", " __TIME__) << std::endl;
	std::cout << "By Gh0stBlade" << std::endl;
}

void PrintToolUsage()
{
	std::cout << "DRM.exe [input.drm]" << std::endl;
	system("pause");
}