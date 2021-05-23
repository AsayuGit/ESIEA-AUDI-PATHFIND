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

#ifndef _TYPES_H
#define _TYPES_H

    #include <SDL2/SDL.h>
    #include <stdbool.h>

    typedef SDL_Texture Surface;
    #define SDL_HWSURFACE SDL_SWSURFACE

    typedef struct{
        int x;
        int y;
    } Vector2i;

    typedef struct{
        double x;
        double y;
    } Vector2d;

    typedef struct{
        char* TileMapPath;
        Surface* TileMapSurface;
        unsigned int MapSizeX;
        unsigned int MapSizeY;
        unsigned int MapSize;
        Uint32 ColorKey;
    } TileMap;

    typedef struct{
        int data;
        struct IntLinkedList* next;
    }IntLinkedList;

    typedef struct{
        Vector2i data;
        struct Vector2iLinkedList* next;
    }Vector2iLinkedList;

    typedef struct{
        unsigned int** MapData;
        unsigned int MapSizeX;
        unsigned int MapSizeY;
        SDL_Rect MapRegion;
        TileMap MapTileMap;
        IntLinkedList* forbiddenTiles;
    } Map;

    typedef struct{
        SDL_Window *Screen;
        SDL_Renderer *Renderer;
        Vector2i ScreenResolution;
        SDL_Rect Camera;
    } DisplayDevice;

    typedef struct{
        /* Events */
        SDL_Event event;

        /* Key Presses */
        const Uint8* KeyStates; /* Pointer to the keypresses */
    } InputDevice;

    typedef struct{
        SDL_Surface* FontSurface;
        SDL_Texture* FontTexture;
        unsigned int FontHeight;
    } BitmapFont;

    Vector2i InitVector2i(int x, int y);
    Vector2d InitVector2d(double x, double y);
    SDL_Rect InitSDL_Rect(int x, int y, int w, int h);
    void AddToIntLinkedList(IntLinkedList** List, int data);
    void AddToVector2iLinkedList(Vector2iLinkedList** List, Vector2i data);
    void FreeVector2iLinkedList(Vector2iLinkedList* List);

#endif
