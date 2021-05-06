#include<stdlib.h>
#include<stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "system.h"
#include "load.h"
#include "graphics.h"
#include "map.h"
#include "debug.h"
#include "keymap.h"

void mainGame(DisplayDevice* DDevice, InputDevice* IDevice){
    /* Declaration */
    Map* WorldMap;
    bool DebugMode;

    /* Init */
    InitDebug(DDevice);
    DebugMode = false;
    
    WorldMap = LoadMap(DDevice, "Assets/WorldMaps/OverWorld.txt");

    /* Main Game loop */
    while (true){
        /* Events Loop */
        while(SDL_PollEvent(&IDevice->event)){
            /* Global events */
            switch (IDevice->event.type)
            {
            case SDL_QUIT:
                goto Exit;
                break;
            default:
                break;
            }

            /* Regular events */
            if (IDevice->EventEnabled){
                switch (IDevice->event.type){
                case SDL_KEYDOWN:
                    switch (IDevice->event.PADKEY)
                    {
                    case SDL_SCANCODE_ESCAPE:
                        DebugMode = (DebugMode) ? false : true;
                        break;
                    default:
                        break;
                    }
                break;
                default:
                    break;
                }
            }
            if (DebugMode)
                DebugEvents(DDevice, IDevice, WorldMap);
        }
        
        /* Draw World Map */ /* How about we put that in a buffer ? but that would make it static :/ */
        DisplayWorldMap(DDevice, WorldMap);
        if (DebugMode)
            DisplayMapEditor(DDevice);
        SDL_RenderPresent(DDevice->Renderer);
    }
    
Exit:
    /* Free */
    printf("By By\n");
}

int main(int argc, char* argv[]){
    /* Declaration */
    DisplayDevice* DDevice;
    InputDevice* IDevice;
    
    /* Init */
    InitSDL();
    DDevice = CreateDisplayDevice(640, 480, "Audi RO");
    IDevice = InitInputs(false);
    CreateSoundDevice();
    
    /* Game */
    mainGame(DDevice, IDevice);

    /* Free */
    free(IDevice);
    free(DDevice);
    
    return 0;
}
