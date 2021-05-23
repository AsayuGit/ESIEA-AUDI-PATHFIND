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

#ifndef _GRAPHICS_H
#define _GRAPHICS_H

    #include "types.h"

    void DrawTile(DisplayDevice* DDevice, TileMap* Map, unsigned int TileID, unsigned int X, unsigned int Y);
    void DisplayWorldMap(DisplayDevice* DDevice, Map* WorldMap);
    unsigned int BiggestRectMultiple(SDL_Rect* ParentRect, SDL_Rect* ChildRect);
    void ScaleToBiggestRectMultiple(SDL_Rect* ParentRect, SDL_Rect* ChildRect);
    bool RectOnRect(const SDL_Rect* SrcRect, const SDL_Rect* DstRect);
    bool RectOnScreen(DisplayDevice* DDevice, const SDL_Rect* Rect);
    void DrawFrame(DisplayDevice* DDevice);
    
#endif