/*
	[DRM] Tomb Raider: Legend/Anniversary/Underworld DRM Unpacker
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

#ifndef FILEEXTENSIONS_H
#define FILEEXTESNIONS_H

#if TR7

char* szExtensions[]
{
	    ".gnc",
		".ept",
		".ani",
		".pbrwc",
		".pbr",
		".pcd",
		".fsb",
		".dtp",
		".script",
		".shad",
		".nst"
};

#elif TR8
	#if PC
		char* szExtensions[]
		{
			".gnc",
			".unk01",
			".ani",
			".unk03",
			".unk04",
			".tr8pcd9",
			".fsb",
			".dtp",
			".script",
			".shad",
			".matd",
			".obj",
			".tr8mesh",
			".unk13"
		};
	#elif XENON
		char* szExtensions[]
		{
				".gnc",
				".unk01",
				".ani",
				".unk03",
				".unk04",
				".tr8x360",
				".fsb",
				".dtp",
				".script",
				".shad",
				".matd",
				".obj",
				".tr8mesh",
				".unk13"
		};
	#elif PS3
		char* szExtensions[]
		{
			"	.gnc",
				".unk01",
				".ani",
				".unk03",
				".unk04",
				".tr8ps3t",
				".fsb",
				".dtp",
				".script",
				".shad",
				".matd",
				".obj",
				".tr8mesh",
				".unk13"
		};
	#elif WII
		char* szFileExtensions[]
		{
				".gnc",
				".unk01",
				".ani",
				".unk03",
				".unk04",
				".tr8sgl",
				".fsb",
				".dtp",
				".script",
				".shad",
				".matd",
				".obj",
				".tr8mesh",
				".unk13"
		};
	#elif PS2
		char* szFileExtensions[]
		{
				".gnc",
				".unk01",
				".ani",
				".unk03",
				".unk04",
				".tr8sgl",
				".fsb",
				".dtp",
				".script",
				".shad",
				".matd",
				".obj",
				".tr8mesh",
				".unk13"
		};
	#else
		#error "Unsupported Platform!"
	#endif
#else
	#error "Unsupported Game!"
#endif

#endif