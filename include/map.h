#ifndef _MAP_H
#define _MAP_H

    #include "types.h"

    Map* LoadMap(DisplayDevice* DDevice, char* MapFilePath);
    void SaveMap(Map* MapToSave, char* MapFilePath);
    bool IsPosValid(Map* Worldmap, int X, int Y);
    void nextValidPosition(Vector2d* PlayerPos, Map* WorldMap, double deltaX, double deltaY);
    
#endif