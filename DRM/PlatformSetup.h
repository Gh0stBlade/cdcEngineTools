/*
	[DRM/CDRM] Tomb Raider: Legend/Anniversary/Underworld DRM/CDRM Unpacker
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

#ifndef PLATFORM_H
#define PLATFORM_H

#if PC 
#define PLATFORM_FULL_NAME "PC"
#define PLATFORM_CONFIG_NAME "PC-W"
#elif PS2 
#define PLATFORM_FULL_NAME "PS2"
#define PLATFORM_CONFIG_NAME "PS2-W"
#define ENDIAN_BIG (0)
#elif PSP 
#define PLATFORM_FULL_NAME "PSP"
#define PLATFORM_CONFIG_NAME "PSP-W"
#define ENDIAN_BIG (0)
#elif WII 
#define PLATFORM_FULL_NAME "WII"
#define PLATFORM_CONFIG_NAME "WII-W"
#define ENDIAN_BIG (0)
#elif PSP 
#define PLATFORM_FULL_NAME "PSP"
#define PLATFORM_CONFIG_NAME "PSP-W"
#define ENDIAN_BIG (0)
#elif XENON 
#define PLATFORM_FULL_NAME "XENON"
#define PLATFORM_CONFIG_NAME "XENON-W"
#define ENDIAN_BIG (1)
#elif PS3
#define PLATFORM_FULL_NAME "PS3"
#define PLATFORM_CONFIG_NAME "PS3-W"
#define ENDIAN_BIG (1)
#elif NGC
#define PLATFORM_FULL_NAME "NGC"
#define PLATFORM_CONFIG_NAME "NGC-W"
#define ENDIAN_BIG (1)
#else
//Error

#endif

#endif