#ifndef PLATFORM_H
#define PLATFORM_H

#if TR8
#if PC 
	#define PLATFORM_FULL_NAME "PC"
	#define PLATFORM_CONFIG_NAME "PC-W"
	#define ENDIAN_BIG (0)
#elif PS2 
	#define PLATFORM_FULL_NAME "PS2"
	#define PLATFORM_CONFIG_NAME "PS2-W"
	#define ENDIAN_BIG (0)
#elif WII 
	#define PLATFORM_FULL_NAME "WII"
	#define PLATFORM_CONFIG_NAME "WII-W"
	#define ENDIAN_BIG (0)
#elif XENON 
	#define PLATFORM_FULL_NAME "XENON"
	#define PLATFORM_CONFIG_NAME "XENON-W"
	#define ENDIAN_BIG (1)
#elif PS3
	#define PLATFORM_FULL_NAME "PS3"
	#define PLATFORM_CONFIG_NAME "PS3-W"
	#define ENDIAN_BIG (1)
#else
	#error "Unsupported Platform!"
#endif
#else
	#error "Unsupported Game!"
#endif

#endif