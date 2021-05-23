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
#include "graphics.h"

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

void UpdateResolution(DisplayDevice* DDevice){
    int ScreenWidth, ScreenHeight;

    SDL_GetWindowSize(DDevice->Screen, &ScreenWidth, &ScreenHeight);

    DDevice->IRScalar = MAX(MIN(ScreenWidth / BASE_RESOLUTION_X, ScreenHeight / BASE_RESOLUTION_Y), 1);

    DDevice->ScreenResolution.x = ScreenWidth;
    DDevice->ScreenResolution.y = ScreenHeight;

    DDevice->InternalResolution.w = BASE_RESOLUTION_X * DDevice->IRScalar;
    DDevice->InternalResolution.h = BASE_RESOLUTION_Y * DDevice->IRScalar;

    DDevice->InternalResolution.x = (DDevice->ScreenResolution.x - DDevice->InternalResolution.w) >> 1;
    DDevice->InternalResolution.y = (DDevice->ScreenResolution.y - DDevice->InternalResolution.h) >> 1;

    DDevice->Frame[0] = InitRect(0, 0, DDevice->InternalResolution.x, ScreenHeight);                                                                                                            /* Left Frame */
    DDevice->Frame[1] = InitRect(DDevice->InternalResolution.x + DDevice->InternalResolution.w, 0, DDevice->InternalResolution.x, ScreenHeight);                                                /* Right Frame */
    DDevice->Frame[2] = InitRect(DDevice->InternalResolution.x, 0, DDevice->InternalResolution.w, DDevice->InternalResolution.y);                                                               /* Top Frame */
    DDevice->Frame[3] = InitRect(DDevice->InternalResolution.x, DDevice->InternalResolution.y + DDevice->InternalResolution.h, DDevice->InternalResolution.w, DDevice->InternalResolution.y);   /* Bottom Frame */
}

DisplayDevice* CreateDisplayDevice(int ScreenWidth, int ScreenHeight, char* Title){
    DisplayDevice* Device = (DisplayDevice*)malloc(sizeof(DisplayDevice));

    Device->Screen = SDL_CreateWindow(Title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ScreenWidth, ScreenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (Device->Screen == NULL){
        fprintf(stderr, "Can't create main window\n - %s\n", SDL_GetError());
        exit(-1);
    }

    Device->Renderer = SDL_CreateRenderer(Device->Screen , -1, 0);
    if (Device->Renderer == NULL){
        fprintf(stderr, "Can't create main renderer\n - %s\n", SDL_GetError());
        exit(-1);
    }
    SDL_GL_SetSwapInterval(1); /* VSync */

    Device->ScreenResolution = InitVector2i(ScreenWidth, ScreenHeight);
    Device->Camera = InitSDL_Rect(0, 0, BASE_RESOLUTION_X, BASE_RESOLUTION_Y);

    UpdateResolution(Device);

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

    DDevice->Camera.x = PlayerPosition.x - (DDevice->Camera.w >> 1);
    DDevice->Camera.y = PlayerPosition.y - (DDevice->Camera.h >> 1);

    BoundCameraToRegion(DDevice, WorldMap->MapRegion);
}

char* astrcpy(char** dst, char* src){
    size_t length;

    length = strlen(src)+1;                     /* First we compute how long is the input string */
    *dst = (char*)malloc(length*sizeof(char));   /* Then we allocate the right amount of memory */
    memcpy(*dst, src, length);                  /* Finally we copy the content from the source to the destination */

    return *dst;
}

int DrawEx(DisplayDevice* DDevice, SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect, bool flip){
    return SDL_RenderCopyEx(DDevice->Renderer, texture, srcrect, dstrect, 0, 0, flip);
}

int Draw(DisplayDevice* DDevice, SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect){
    return DrawEx(DDevice, texture, srcrect, dstrect, 0);
}

int ScaledDrawEx(DisplayDevice* DDevice, SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect, bool flip){
    SDL_Rect ScaledDstRect = {0, 0, BASE_RESOLUTION_X, BASE_RESOLUTION_Y};

    if (texture && RectOnScreen(DDevice, dstrect)){

        if (dstrect){
            ScaledDstRect = InitRect(
                (dstrect->x * DDevice->IRScalar) + DDevice->InternalResolution.x,
                (dstrect->y * DDevice->IRScalar) + DDevice->InternalResolution.y,
                dstrect->w * DDevice->IRScalar,
                dstrect->h * DDevice->IRScalar
            );
        }
        return DrawEx(DDevice, texture, srcrect, &ScaledDstRect, flip);
    }
    return 0;
}

int ScaledDraw(DisplayDevice* DDevice, SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect){
    return ScaledDrawEx(DDevice, texture, srcrect, dstrect, false);
}

void SystemEvents(DisplayDevice* DDevice, InputDevice* IDevice){
    switch (IDevice->event.type){
        case SDL_WINDOWEVENT:
            switch (IDevice->event.window.event)
            {
            case SDL_WINDOWEVENT_RESIZED:
                UpdateResolution(DDevice);
                break;
            
            default:
                break;
            }
            break;

        case SDL_KEYDOWN:
            switch (IDevice->event.key.keysym.scancode)
            {
            
            case SDL_SCANCODE_F:
                printf("NYAN\n");
                SDL_SetWindowFullscreen(DDevice->Screen, (SDL_GetWindowFlags(DDevice->Screen) & SDL_WINDOW_FULLSCREEN_DESKTOP) ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
                break;

            case SDL_SCANCODE_F1:
                SDL_SetWindowSize(DDevice->Screen, BASE_RESOLUTION_X * DDevice->IRScalar, BASE_RESOLUTION_Y * DDevice->IRScalar);
                UpdateResolution(DDevice);
                break;

            default:
                break;
            }
            break;

        default:
            break;
    }

}

void FinishFrame(DisplayDevice* DDevice){
    DrawFrame(DDevice);
    SDL_RenderPresent(DDevice->Renderer);
}