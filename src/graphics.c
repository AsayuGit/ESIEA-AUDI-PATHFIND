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

#include "graphics.h"
#include "defines.h"

void DrawTile(DisplayDevice* DDevice, TileMap* Map, unsigned int TileID, unsigned int X, unsigned int Y){
    /* Declaration */
    SDL_Rect SrcTile, DstTile;

    /* Init */
    SrcTile.x = (TileID % Map->MapSizeX) * TILE_SIZE;
    SrcTile.y = (TileID / Map->MapSizeX) * TILE_SIZE;
    SrcTile.w = TILE_SIZE;
    SrcTile.h = TILE_SIZE;

    DstTile.x = X * TILE_SIZE - DDevice->Camera.x;
    DstTile.y = Y * TILE_SIZE - DDevice->Camera.y;
    DstTile.w = TILE_SIZE;
    DstTile.h = TILE_SIZE;
    /* Logic */

    SDL_RenderCopy(DDevice->Renderer, Map->TileMapSurface, &SrcTile, &DstTile);
}

void DisplayWorldMap(DisplayDevice* DDevice, Map* WorldMap){
    /* Declaration */
    unsigned int i, j;

    /* Logic */
    for (i = 0; i < WorldMap->MapSizeY; i++){
        for (j = 0; j < WorldMap->MapSizeX; j++){
            DrawTile(DDevice, &WorldMap->MapTileMap, WorldMap->MapData[i][j], j, i);
        }
    }
}

unsigned int BiggestRectMultiple(SDL_Rect* ParentRect, SDL_Rect* ChildRect){
    return MIN(ParentRect->w / ChildRect->w, ParentRect->h / ChildRect->h);
}

void ScaleToBiggestRectMultiple(SDL_Rect* ParentRect, SDL_Rect* ChildRect){
    unsigned int biggestMultiple;

    biggestMultiple = BiggestRectMultiple(ParentRect, ChildRect);
    ChildRect->w *= biggestMultiple;
    ChildRect->h *= biggestMultiple;
}