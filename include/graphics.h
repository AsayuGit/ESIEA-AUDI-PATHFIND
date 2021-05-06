#ifndef _GRAPHICS_H
#define _GRAPHICS_H

    #include "types.h"

    void DrawTile(DisplayDevice* DDevice, TileMap* Map, unsigned int TileID, unsigned int X, unsigned int Y);
    void DisplayWorldMap(DisplayDevice* DDevice, Map* WorldMap);
    void DisplayCharacter(DisplayDevice* DDevice);
    
#endif