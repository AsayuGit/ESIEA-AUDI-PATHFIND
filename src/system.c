#include "system.h"
#include "defines.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

void InitSDL(){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        fprintf(stderr, "SDL Initialisation failed\n - %s\n", SDL_GetError());
        exit(-1);
    }
}

InputDevice* InitInputs(bool JoyEnabled){
    InputDevice* Inputs = (InputDevice*)malloc(sizeof(InputDevice));
    Inputs->Joy1 = NULL;
    if (SDL_NumJoysticks() && JoyEnabled){
        Inputs->Joy1 = SDL_JoystickOpen(0); /* Open Joystick */
    }
    Inputs->KeyStates = SDL_GetKeyboardState(NULL); /* Open Keyboard */
    if (Inputs->Joy1 == NULL){
        Inputs->JoyEnabled = false;
    }else{
        Inputs->JoyEnabled = true;
    }
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
    /* InitJukebox(); */ /* Countains all music to be played */
    /* return NULL; */
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

void CenterCameraOn(DisplayDevice* DDevice, Map* WorldMap, int x, int y){
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