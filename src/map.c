#include "map.h"

#include <limits.h>
#include "types.h"
#include "defines.h"
#include "Load.h"
#include "system.h"
#include "aStar.h"

static Vector2iLinkedList* CurrentPath;

void FreeMap(Map* MapToFree); /* FIXME */

/* Load a map in memory from a file */
Map* LoadMap(DisplayDevice* DDevice, char* MapFilePath){
    /* Declaration */
    Map* LoadedMap;
    FILE* MapFile;
    unsigned int i, j, Buffer;
    char TileMapPath[PATH_MAX];

    /* Init */
    MapFile = fopen(MapFilePath, "r");
    if (!MapFile){
        printf("Couldn't load map file: %s !\n", MapFilePath);
        goto Error;
    }
    LoadedMap = (Map*)malloc(sizeof(Map));

    /* Logic */
    fscanf(MapFile, "%u %u %u %u\n%x\n", &LoadedMap->MapSizeX, &LoadedMap->MapSizeY, &LoadedMap->MapTileMap.MapSizeX, &LoadedMap->MapTileMap.MapSizeY, &LoadedMap->MapTileMap.ColorKey);
    LoadedMap->MapTileMap.MapSize = LoadedMap->MapTileMap.MapSizeX * (LoadedMap->MapTileMap.MapSizeY - 1);
    
    fgets(TileMapPath, PATH_MAX, MapFile);
    TileMapPath[strcspn(TileMapPath, "\n")] = '\0';

    LoadedMap->MapTileMap.TileMapPath = (char*)malloc(sizeof(char)*(strlen(TileMapPath) + 1));
    strcpy(LoadedMap->MapTileMap.TileMapPath, TileMapPath);
    
    LoadedMap->MapTileMap.TileMapSurface = LoadSurface(LoadedMap->MapTileMap.TileMapPath, DDevice, LoadedMap->MapTileMap.ColorKey, false);
    
    /* printf("Map size %u %u \n", LoadedMap->MapSizeX, LoadedMap->MapSizeY); */
    LoadedMap->MapData = (unsigned int**)malloc(sizeof(unsigned int*)*LoadedMap->MapSizeY);
    
    for (i = 0; i < LoadedMap->MapSizeY; i++){
        LoadedMap->MapData[i] = (unsigned int*)malloc(sizeof(unsigned int)*LoadedMap->MapSizeX);
        for (j = 0; j < LoadedMap->MapSizeX; j++){
            fscanf(MapFile, "%u", &LoadedMap->MapData[i][j]);
        }
    }

    LoadedMap->MapRegion = InitSDL_Rect(0, 0, LoadedMap->MapSizeX * TILE_SIZE, LoadedMap->MapSizeY * TILE_SIZE);

    LoadedMap->forbiddenTiles = NULL;
    while (fscanf(MapFile, "%u", &Buffer) != EOF){
        AddToIntLinkedList(&LoadedMap->forbiddenTiles, Buffer);
    }

    /* free */
Error:
    if (MapFile)
        fclose(MapFile);

    return LoadedMap;
}

/* Save a map form memory to a file */
void SaveMap(Map* MapToSave, char* MapFilePath){
    /* Declaration */
    FILE* MapFile;
    unsigned int i, j;
    IntLinkedList* FBTiles;

    /* Init */
    MapFile = fopen(MapFilePath, "w");
    if (!MapFile){
        printf("Couldn't load map file: %s !\n", MapFilePath);
        goto Error;
    }

    /* Logic */

    /* Print the header to file */
    fprintf(MapFile, "%u %u %u %u\n0x%x\n%s\n", MapToSave->MapSizeX, MapToSave->MapSizeY, MapToSave->MapTileMap.MapSizeX, MapToSave->MapTileMap.MapSizeY, MapToSave->MapTileMap.ColorKey, MapToSave->MapTileMap.TileMapPath);

    /* Print map data */
    for (i = 0; i < MapToSave->MapSizeY; i++){
        for (j = 0; j < MapToSave->MapSizeX; j++){
            fprintf(MapFile, "%u ", MapToSave->MapData[i][j]);
        }
        fprintf(MapFile, "\n");
    }

    FBTiles = MapToSave->forbiddenTiles;
    while (FBTiles){
        fprintf(MapFile, "%d ", FBTiles->data);
        FBTiles = (IntLinkedList*)FBTiles->next;
    }
    fprintf(MapFile, "\n");

    /* free */
Error:
    if (MapFile)
        fclose(MapFile);
}

bool IsPosValid(Map* WorldMap, int X, int Y){
    IntLinkedList* List;

    List = WorldMap->forbiddenTiles;
    
    if ((X >= WorldMap->MapSizeX) || (Y >= WorldMap->MapSizeY) || (X < 0) || (Y < 0)) /* Invalid Coordinates */
        return false;

    while (List){
        if ((WorldMap->MapData[Y][X] == List->data)) /* Invalid Tile */
            return false;
        List = (IntLinkedList*)List->next;
    }
    return true;
}

void nextValidPosition(Vector2d* PlayerPos, Map* WorldMap, double deltaX, double deltaY){
    Vector2i MapPlayerPos, NextMapPlayerPos;

    MapPlayerPos.x = (int)(PlayerPos->x / TILE_SIZE);
    MapPlayerPos.y = (int)(PlayerPos->y / TILE_SIZE);

    NextMapPlayerPos.x = (int)((PlayerPos->x + deltaX) / TILE_SIZE);
    NextMapPlayerPos.y = (int)((PlayerPos->y + deltaY) / TILE_SIZE);

    if (IsPosValid(WorldMap, NextMapPlayerPos.x, MapPlayerPos.y)){
        PlayerPos->x += deltaX;
    } else {
        /* snap to edge */
    }

    if (IsPosValid(WorldMap, MapPlayerPos.x, NextMapPlayerPos.y)){
        PlayerPos->y += deltaY;
    } else {
        /* snap to edge */
    }

    BoundVect2dToRegion(PlayerPos, WorldMap->MapRegion);
}


void setPath(Vector2iLinkedList* Path){
    CurrentPath = Path;
}   

/* Linear interpolation between two points */
bool WallOnPath(Vector2d* PlayerPosition, double PlayerSpeed, CharacterList* CharaHandle){
    Vector2d Distance; /* Vecteur direction */
    Vector2d Destination;
    double euclidianDistance;

    if (CurrentPath){
        Destination.x = (CurrentPath->data.x * TILE_SIZE) + 16;
        Destination.y = (CurrentPath->data.y * TILE_SIZE) + 16;

        Distance.x = Destination.x - PlayerPosition->x;
        Distance.y = Destination.y - PlayerPosition->y;

        /* Animation */
        if (abs(Distance.y) > abs(Distance.x)){
            if (Distance.y > 0){
                CharacterPlayAnimation(CharaHandle->Character, 3, false);
                CharaHandle->Flip = false;
            } else {
                CharacterPlayAnimation(CharaHandle->Character, 4, false);
                CharaHandle->Flip = false;
            }
        } else {
            if (Distance.x > 0){
                CharacterPlayAnimation(CharaHandle->Character, 5, false);
                CharaHandle->Flip = false;
            } else {
                CharacterPlayAnimation(CharaHandle->Character, 5, false);
                CharaHandle->Flip = true;
            }
        }

        euclidianDistance = sqrt((Distance.x*Distance.x) + (Distance.y*Distance.y));

        Distance.x = Distance.x / euclidianDistance * PlayerSpeed;
        Distance.y = Distance.y / euclidianDistance * PlayerSpeed;

        if (euclidianDistance < PlayerSpeed){
            PlayerPosition->x = Destination.x;
            PlayerPosition->y = Destination.y;
            CurrentPath = (Vector2iLinkedList*)CurrentPath->next;
        } else {
            PlayerPosition->x += Distance.x;
            PlayerPosition->y += Distance.y;
        }
        return false;
    }
    return true;
}

unsigned int FindPotentialChestLocations(Map* WorldMap, Vector2i** potentialChestLocation){
    Vector2iLinkedList* potentialChestLocationsList = NULL;
    Vector2iLinkedList* potentialChestLocationsListPointer;
    unsigned int X, Y, nbOfPotentialChestLocation = 0;

    for (Y = 0; Y < WorldMap->MapSizeY; Y++){
        for (X = 0; X < WorldMap->MapSizeX; X++){
            if (WorldMap->MapData[Y][X] == 91){
                AddToVector2iLinkedList(&potentialChestLocationsList, InitVector2i(X, Y));
                nbOfPotentialChestLocation++;
            }
        }
    }

    (*potentialChestLocation) = (Vector2i*)malloc(nbOfPotentialChestLocation*sizeof(Vector2i));
    potentialChestLocationsListPointer = potentialChestLocationsList;

    for (X = 0; X < nbOfPotentialChestLocation; X++){
        (*potentialChestLocation)[X] = potentialChestLocationsListPointer->data;
        potentialChestLocationsListPointer = (Vector2iLinkedList*)potentialChestLocationsListPointer->next;
    }

    FreeVector2iLinkedList(potentialChestLocationsList);

    return nbOfPotentialChestLocation;
}

unsigned int GetPathLength(Vector2iLinkedList* path){
    Vector2iLinkedList* pathIterator;
    unsigned int length = 0;

    pathIterator = path;
    while (pathIterator){
        pathIterator = (Vector2iLinkedList*)pathIterator->next;
        length++;
    }
    
    return length;
}

Vector2iLinkedList* GetShortestPath(Vector2iLinkedList** pathArray, unsigned int nbOfPaths){
    unsigned int i, currentPathLength, minPathLength, minPathIndex = 0;

    minPathLength = GetPathLength(pathArray[0]);
    for (i = 1; i < nbOfPaths; i++){
        currentPathLength = GetPathLength(pathArray[i]);
        if (currentPathLength < minPathLength){
            minPathLength = currentPathLength;
            minPathIndex = i;
        }
    }

    return pathArray[minPathIndex];
}

double GetEuclidianDistance(Vector2i Start, Vector2i End){
    Vector2i distance;

    distance.x = End.x - Start.x;
    distance.y = End.y - Start.y;

    return sqrt((distance.x*distance.x) + (distance.y*distance.y));
}

double GetEuclidianPathLength(Vector2iLinkedList* path){
    Vector2iLinkedList* lastNode;
    double length = 0.0f;
    
    if (path){
        do {
            lastNode = path;
            path = (Vector2iLinkedList*)path->next;
            length += GetEuclidianDistance(lastNode->data, path->data);
        } while (path->next);
    }

    return length;
}

Vector2iLinkedList* GetShortestEuclidianPath(Vector2iLinkedList** pathArray, unsigned int nbOfPaths){
    unsigned int i, minPathIndex = 0;
    double currentPathLength, minPathLength;

    minPathLength = GetEuclidianPathLength(pathArray[0]);
    for (i = 1; i < nbOfPaths; i++){
        currentPathLength = GetEuclidianPathLength(pathArray[i]);
        if (currentPathLength < minPathLength){
            minPathLength = currentPathLength;
            minPathIndex = i;
        }
    }

    return pathArray[minPathIndex];
}