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

#include "aStar.h"
#include "internalAStar.h"

/* This function is meant to wrap the internal A* algorythm to a path format
   usable by the game engine */
Vector2iLinkedList* getPath(Map* WorldMap, Vector2i TileStart, Vector2i TileEnd){
    Vector2iLinkedList* returnPath = NULL;
    IntLinkedList* forbiddenTilesPointer = NULL;
    astar_result astarResult;
    unsigned int** BinaryMap;
    unsigned int Y, X;

    /* Binary map generation */
    BinaryMap = (unsigned int**)malloc(WorldMap->MapSizeY*sizeof(unsigned int*));
    for (Y = 0; Y < WorldMap->MapSizeY; Y++){
        BinaryMap[Y] = (unsigned int*)calloc(WorldMap->MapSizeX,sizeof(unsigned int));
        for (X = 0; X < WorldMap->MapSizeX; X++){
            forbiddenTilesPointer = WorldMap->forbiddenTiles;
            while (forbiddenTilesPointer){
                if (WorldMap->MapData[Y][X] == forbiddenTilesPointer->data){
                    BinaryMap[Y][X] = 1;
                    break;
                }
                forbiddenTilesPointer = (IntLinkedList*)forbiddenTilesPointer->next;
            }
        }
    }

    /* A* Call */
    if (astar(BinaryMap, WorldMap->MapSizeX, WorldMap->MapSizeY, TileStart.x, TileStart.y, TileEnd.x, TileEnd.y, &astarResult)){
        for (X = 0; X < astarResult.pathsize; X++){
            AddToVector2iLinkedList(&returnPath, InitVector2i(astarResult.path[X].x, astarResult.path[X].y));
        }
    }

    /* Binary Map Free */
    for (Y = 0; Y < WorldMap->MapSizeY; Y++){
        free(BinaryMap[Y]);
    }
    free(BinaryMap);

    return returnPath;
}