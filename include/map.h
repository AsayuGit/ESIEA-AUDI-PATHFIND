#ifndef _MAP_H
#define _MAP_H

    #include "types.h"
    #include "Characters.h"

    Map* LoadMap(DisplayDevice* DDevice, char* MapFilePath);
    void SaveMap(Map* MapToSave, char* MapFilePath);
    bool IsPosValid(Map* WorldMap, int X, int Y);
    void nextValidPosition(Vector2d* PlayerPos, Map* WorldMap, double deltaX, double deltaY);
    void setPath(Vector2iLinkedList* Path);
    bool WallOnPath(Vector2d* PlayerPosition, double PlayerSpeed, CharacterList* CharaHandle);
    unsigned int FindPotentialChestLocations(Map* WorldMap, Vector2i** potentialChestLocation);
    Vector2iLinkedList* GetShortestPath(Vector2iLinkedList** pathArray, unsigned int nbOfPaths);
    Vector2iLinkedList* GetShortestEuclidianPath(Vector2iLinkedList** pathArray, unsigned int nbOfPaths);
    
#endif