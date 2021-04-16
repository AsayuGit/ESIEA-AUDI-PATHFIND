#include "debug.h"
#include "load.h"
#include "defines.h"
#include "keymap.h"

static Surface* UISurface;
static SDL_Rect CursorSrcRect, CursorDstRect;

static Vector2i CursorPos;

void InitDebug(DisplayDevice* DDevice){
    /* Declaration */
    Uint32 ColorKey;

    /* Init */
    ColorKey = 0xff00ff;

    /* Logic */
    UISurface = LoadSurface("Assets/Textures/Menus/UI.bmp", DDevice, &ColorKey, false);
    
    CursorSrcRect.x = 0;
    CursorSrcRect.y = 0;
    CursorSrcRect.w = 38;
    CursorSrcRect.h = 38;

    CursorPos.x = 0;
    CursorPos.y = 0;
}

void DebugEvents(InputDevice* IDevice, Map* WorldMap){
    if (IDevice->EventEnabled){
        switch (IDevice->event.type){
        case SDL_KEYDOWN:
            switch (IDevice->event.PADKEY)
            {
            case PAD_LEFT:
                if (CursorPos.x > 0)
                    CursorPos.x--;
                break;
            case PAD_RIGHT:
                if (CursorPos.x < WorldMap->MapSizeX)
                    CursorPos.x++;
                break;
            case PAD_UP:
                if (CursorPos.y > 0)
                    CursorPos.y--;
                break;
            case PAD_DOWN:
                if (CursorPos.y < WorldMap->MapSizeY)
                    CursorPos.y++;
                break;
            case PAD_PLUS:
                if (WorldMap->MapData[CursorPos.y][CursorPos.x] < WorldMap->MapTileMap.MapSize - 1)
                    WorldMap->MapData[CursorPos.y][CursorPos.x]++;
                printf("New Tile: %u\n", WorldMap->MapData[CursorPos.y][CursorPos.x]);
                break;
            case PAD_MINUS:
                if (WorldMap->MapData[CursorPos.y][CursorPos.x] > 0)
                    WorldMap->MapData[CursorPos.y][CursorPos.x]--;
                printf("New Tile: %u\n", WorldMap->MapData[CursorPos.y][CursorPos.x]);
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}

void DisplayMapEditor(DisplayDevice* DDevice){
    CursorDstRect.x = CursorPos.x * TILE_SIZE - 4;
    CursorDstRect.y = CursorPos.y * TILE_SIZE - 4;
    CursorDstRect.w = 38;
    CursorDstRect.h = 38;

    SDL_RenderCopy(DDevice->Renderer, UISurface, &CursorSrcRect, &CursorDstRect);
}