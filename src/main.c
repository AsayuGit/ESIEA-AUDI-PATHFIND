#include<stdlib.h>
#include<stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "system.h"
#include "Load.h"
#include "graphics.h"
#include "map.h"
#include "debug.h"
#include "keymap.h"
#include "Characters.h"

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
            switch (IDevice->event.type)
            {
            case SDL_QUIT:
                goto Exit;
                break;

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
            if (DebugMode)
                DebugEvents(DDevice, IDevice, WorldMap);
        }
        
        DisplayWorldMap(DDevice, WorldMap); /* Draw World Map */
        /*DisplayCharacter(DDevice);*/          /* Draw the main character */
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
