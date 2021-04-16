#include "map.h"
#include "types.h"
#include "defines.h"
#include "load.h"

Map* LoadMap(DisplayDevice* DDevice, char* MapFilePath){
    /* Declaration */
    Map* LoadedMap;
    FILE* MapFile;
    unsigned int i, j;
    char TileMapPath[PATH_MAX];
    Uint32 ColorKey;

    /* Init */
    MapFile = fopen(MapFilePath, "r");
    if (!MapFile){
        printf("Couldn't load map file: %s !\n", MapFilePath);
        goto Error;
    }
    LoadedMap = (Map*)malloc(sizeof(Map));

    /* Logic */
    fscanf(MapFile, "%u %u %u\n%x\n", &LoadedMap->MapSizeX, &LoadedMap->MapSizeY, &LoadedMap->MapTileMap.MapSizeX, &ColorKey);
    fgets(TileMapPath, PATH_MAX, MapFile);

    LoadedMap->MapTileMap.TileMapSurface = LoadSurface("Assets/Textures/TileMap/TileMap.bmp", DDevice, &ColorKey, false);
    
    /* printf("Map size %u %u \n", LoadedMap->MapSizeX, LoadedMap->MapSizeY); */
    LoadedMap->MapData = (unsigned int**)malloc(sizeof(unsigned int*)*LoadedMap->MapSizeY);
    
    for (i = 0; i < LoadedMap->MapSizeY; i++){
        LoadedMap->MapData[i] = (unsigned int*)malloc(sizeof(unsigned int)*LoadedMap->MapSizeX);
        for (j = 0; j < LoadedMap->MapSizeX; j++){
            fscanf(MapFile, "%u", &LoadedMap->MapData[i][j]);
        }
    }


    /* free*/
Error:
    if (MapFile)
        fclose(MapFile);

    return LoadedMap;
}

void FreeMap(Map* MapToFree);