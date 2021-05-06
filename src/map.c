#include "map.h"
#include "types.h"
#include "defines.h"
#include "load.h"

/* Load a map in memory from a file */
Map* LoadMap(DisplayDevice* DDevice, char* MapFilePath){
    /* Declaration */
    Map* LoadedMap;
    FILE* MapFile;
    unsigned int i, j;
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
    
    LoadedMap->MapTileMap.TileMapSurface = LoadSurface(LoadedMap->MapTileMap.TileMapPath, DDevice, &LoadedMap->MapTileMap.ColorKey, false);
    
    /* printf("Map size %u %u \n", LoadedMap->MapSizeX, LoadedMap->MapSizeY); */
    LoadedMap->MapData = (unsigned int**)malloc(sizeof(unsigned int*)*LoadedMap->MapSizeY);
    
    for (i = 0; i < LoadedMap->MapSizeY; i++){
        LoadedMap->MapData[i] = (unsigned int*)malloc(sizeof(unsigned int)*LoadedMap->MapSizeX);
        for (j = 0; j < LoadedMap->MapSizeX; j++){
            fscanf(MapFile, "%u", &LoadedMap->MapData[i][j]);
        }
    }

    LoadedMap->MapRegion = InitSDL_Rect(0, 0, LoadedMap->MapSizeX * TILE_SIZE, LoadedMap->MapSizeY * TILE_SIZE);

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

    /* free */
Error:
    if (MapFile)
        fclose(MapFile);
}

void FreeMap(Map* MapToFree);