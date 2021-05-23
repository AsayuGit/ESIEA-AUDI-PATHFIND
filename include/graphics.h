#ifndef _GRAPHICS_H
#define _GRAPHICS_H

    #include "types.h"

    void DrawTile(DisplayDevice* DDevice, TileMap* Map, unsigned int TileID, unsigned int X, unsigned int Y);
    void DisplayWorldMap(DisplayDevice* DDevice, Map* WorldMap);
    unsigned int BiggestRectMultiple(SDL_Rect* ParentRect, SDL_Rect* ChildRect);
    void ScaleToBiggestRectMultiple(SDL_Rect* ParentRect, SDL_Rect* ChildRect);
    
#endif