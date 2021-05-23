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

#include "system.h"
#include "defines.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "Jukebox.h"

void InitSDL(){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        fprintf(stderr, "SDL Initialisation failed\n - %s\n", SDL_GetError());
        exit(-1);
    }
}

InputDevice* InitInputs(bool JoyEnabled){
    InputDevice* Inputs = (InputDevice*)malloc(sizeof(InputDevice));
    Inputs->KeyStates = SDL_GetKeyboardState(NULL); /* Open Keyboard */
    return Inputs;
}

DisplayDevice* CreateDisplayDevice(int ScreenWidth, int ScreenHeight, char* Title){
    DisplayDevice* Device = (DisplayDevice*)malloc(sizeof(DisplayDevice));
    
    #ifdef _SDL
        Device->Screen = SDL_SetVideoMode(ScreenWidth, ScreenHeight, 32, SDL_HWSURFACE); /* | SDL_RESIZABLE */
        SDL_WM_SetCaption(Title, NULL);
	    SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1); /* VSync */
    #else
        Device->Screen = SDL_CreateWindow(Title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ScreenWidth, ScreenHeight, SDL_WINDOW_SHOWN);
    #endif
    if (Device->Screen == NULL){
        fprintf(stderr, "Can't create main window\n - %s\n", SDL_GetError());
        exit(-1);
    }
    #ifndef _SDL
        Device->Renderer = SDL_CreateRenderer(Device->Screen , -1, 0);
        if (Device->Renderer == NULL){
            fprintf(stderr, "Can't create main renderer\n - %s\n", SDL_GetError());
            exit(-1);
        }
        SDL_GL_SetSwapInterval(1); /* VSync */
    #endif

    Device->ScreenResolution = InitVector2i(ScreenWidth, ScreenHeight);
    Device->Camera = InitSDL_Rect(0, 0, ScreenWidth, ScreenHeight);

    return Device;
}

void CreateSoundDevice(){
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 512) < 0){
        fprintf(stderr, "Can't create main renderer\n - %s\n", SDL_GetError());
        exit(-1);
    }
}

void BoundCameraToRegion(DisplayDevice* DDevice, SDL_Rect Region){
    
    if (DDevice->Camera.x + DDevice->Camera.w > Region.x + Region.w){
        DDevice->Camera.x = Region.x + Region.w - DDevice->Camera.w;
    }
    if (DDevice->Camera.x < Region.x){
        DDevice->Camera.x = Region.x;
    }

    if (DDevice->Camera.y + DDevice->Camera.h > Region.y + Region.h){
        DDevice->Camera.y = Region.y + Region.h - DDevice->Camera.h;
    }
    if (DDevice->Camera.y < Region.y){
        DDevice->Camera.y = Region.y;
    }

}

void BoundVect2dToRegion(Vector2d* Vect, SDL_Rect Region){
    if (Vect->x > Region.x + Region.w){
        Vect->x = Region.x + Region.w;
    }
    if (Vect->x < Region.x){
        Vect->x = Region.x;
    }

    if (Vect->y > Region.y + Region.h){
        Vect->y = Region.y + Region.h;
    }
    if (Vect->y < Region.y){
        Vect->y = Region.y;
    }
}

Vector2i GetCameraPos(DisplayDevice* DDevice){
    Vector2i CameraPos;

    CameraPos = InitVector2i(
        DDevice->Camera.x / TILE_SIZE,
        DDevice->Camera.y / TILE_SIZE
    );

    return CameraPos;
}

void CenterCameraOnTile(DisplayDevice* DDevice, Map* WorldMap, int x, int y){
    Vector2i CameraTileSize;
    Vector2i NewCameraPos;

    CameraTileSize = InitVector2i(DDevice->Camera.w / TILE_SIZE, DDevice->Camera.h / TILE_SIZE);
    NewCameraPos = InitVector2i(
        (x - (CameraTileSize.x >> 1)) * TILE_SIZE + (TILE_SIZE >> 1),
        (y - (CameraTileSize.y >> 1)) * TILE_SIZE + (TILE_SIZE >> 1)
    );

    DDevice->Camera.x = NewCameraPos.x;
    DDevice->Camera.y = NewCameraPos.y;

    BoundCameraToRegion(DDevice, WorldMap->MapRegion);
}

void CenterCameraOnPlayer(DisplayDevice* DDevice, Map* WorldMap, Vector2d PlayerPosition){

    DDevice->Camera.x = PlayerPosition.x - (DDevice->ScreenResolution.x >> 1);
    DDevice->Camera.y = PlayerPosition.y - (DDevice->ScreenResolution.y >> 1);

    BoundCameraToRegion(DDevice, WorldMap->MapRegion);
}

char* astrcpy(char** dst, char* src){
    size_t length;

    length = strlen(src)+1;                     /* First we compute how long is the input string */
    *dst = (char*)malloc(length*sizeof(char));   /* Then we allocate the right amount of memory */
    memcpy(*dst, src, length);                  /* Finally we copy the content from the source to the destination */

    return *dst;
}