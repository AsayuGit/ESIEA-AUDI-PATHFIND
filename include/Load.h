/*
    PATHFIND is a simple game where the player aims to find chests either
    by themself or by using the included pathfinding algorythm

    Copyright (C) 2021 Killian RAIMBAUD (killian.rai@gmail.com)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; version 2 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef _LOAD_H
#define _LOAD_H
	
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_mixer.h>

	#ifdef _XBOX
		#include <libxml/parser.h>
	#else
		#include <libxml2/libxml/parser.h>
	#endif

    #include "types.h"

    enum {
        SURFACE_OPAQUE = 0,
        SURFACE_ALPHA = 1,
        SURFACE_KEYED = 2
    };

    SDL_Surface* LoadSDLSurface(char FilePath[], DisplayDevice* Device);
    void KeySurface(SDL_Surface* SurfaceToKey, Uint32 ColorKey);
    SDL_Texture* LoadSurface(char FilePath[], DisplayDevice* Device, Uint32 ColorKey, char flags);

    SDL_Texture* CreateTargetSurface(DisplayDevice* DDevice, int w, int h);

    Mix_Music* LoadMusic(const char FilePath[]);
    Mix_Chunk* LoadSoundEffect(char FilePath[]);

    xmlDoc* loadXml(char* filePath);

#endif