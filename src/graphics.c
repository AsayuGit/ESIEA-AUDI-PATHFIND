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