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

#include "debug.h"
#include "Load.h"
#include "defines.h"
#include "keymap.h"
#include "map.h"
#include "system.h"

static Surface* UISurface;
static SDL_Rect CursorSrcRect, CursorDstRect;

static Vector2i CursorPos;

unsigned int clipboard;

void InitDebug(DisplayDevice* DDevice){
    /* Logic */
    UISurface = LoadSurface("Assets/Textures/Menus/UI.bmp", DDevice, 0xff00ff, SURFACE_KEYED);
    
    CursorSrcRect.x = 0;
    CursorSrcRect.y = 0;
    CursorSrcRect.w = 38;
    CursorSrcRect.h = 38;

    CursorPos.x = 0;
    CursorPos.y = 0;

    clipboard = 0;
}

void DebugEvents(DisplayDevice* DDevice, InputDevice* IDevice, Map* WorldMap){
    switch (IDevice->event.type){
    case SDL_KEYDOWN:
        switch (IDevice->event.PADKEY)
        {
        /* Move the cursor around */
        case PAD_LEFT:
            if (CursorPos.x > 0)
                CursorPos.x--;
            break;
        case PAD_RIGHT:
            if (CursorPos.x + 1 < WorldMap->MapSizeX)
                CursorPos.x++;
            break;
        case PAD_UP:
            if (CursorPos.y > 0)
                CursorPos.y--;
            break;
        case PAD_DOWN:
            if (CursorPos.y + 1 < WorldMap->MapSizeY)
                CursorPos.y++;
            break;
        
        /* Increment or decrement a tile */
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

        /* Copy and paste */
        case SDL_SCANCODE_C:
            clipboard = WorldMap->MapData[CursorPos.y][CursorPos.x];
            printf("Tile copied\n");
            break;

        case SDL_SCANCODE_V:
            WorldMap->MapData[CursorPos.y][CursorPos.x] = clipboard;
            printf("Tile pasted\n");
            break;

        /* Save changes */
        case SDL_SCANCODE_F1:
            SaveMap(WorldMap, "Assets/WorldMaps/OverWorld.txt");
            printf("Map Saved !\n");
            break;
        default:
            break;
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        switch (IDevice->event.MICEKEY)
        {
        case SDL_BUTTON_LEFT:
            CursorPos.x = (IDevice->event.motion.x + (DDevice->Camera.x * DDevice->IRScalar) - DDevice->InternalResolution.x) / (TILE_SIZE * DDevice->IRScalar);
            CursorPos.y = (IDevice->event.motion.y + (DDevice->Camera.y * DDevice->IRScalar) - DDevice->InternalResolution.y) / (TILE_SIZE * DDevice->IRScalar);
            printf("Selected : X=%d Y=%d Type=%d\n", CursorPos.x, CursorPos.y, WorldMap->MapData[CursorPos.y][CursorPos.x]);
            break;
        default:
            break;
        }
        break;

    case SDL_MOUSEWHEEL:
        if (IDevice->event.wheel.y > 0){
            if (WorldMap->MapData[CursorPos.y][CursorPos.x] < WorldMap->MapTileMap.MapSize - 1)
                WorldMap->MapData[CursorPos.y][CursorPos.x]++;
            printf("New Tile: %u\n", WorldMap->MapData[CursorPos.y][CursorPos.x]);
        } else if (IDevice->event.wheel.y < 0) {
            if (WorldMap->MapData[CursorPos.y][CursorPos.x] > 0)
                WorldMap->MapData[CursorPos.y][CursorPos.x]--;
            printf("New Tile: %u\n", WorldMap->MapData[CursorPos.y][CursorPos.x]);
        }
        break;

    default:
        break;
    }

    CenterCameraOnTile(DDevice, WorldMap, CursorPos.x, CursorPos.y);
}

void DisplayMapEditor(DisplayDevice* DDevice){
    CursorDstRect.x = CursorPos.x * TILE_SIZE - 3 - DDevice->Camera.x;
    CursorDstRect.y = CursorPos.y * TILE_SIZE - 3 - DDevice->Camera.y;
    CursorDstRect.w = 38;
    CursorDstRect.h = 38;

    ScaledDraw(DDevice, UISurface, &CursorSrcRect, &CursorDstRect);
}

void SetDebugCursorPos(Vector2i NewCursorPos){
    CursorPos = NewCursorPos;
}