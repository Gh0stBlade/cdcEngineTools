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

#ifndef GAME_H
#define GAME_H

#if TR7
#define GAME_NAME "Tomb Raider: Legend"
#elif TRAE
#define GAME_NAME "Tomb Raider: Anniversary"
#elif TR8
#define GAME_NAME "Tomb Raider: Underworld"
#else
#error "Unsupported Game!"
#endif
#endif