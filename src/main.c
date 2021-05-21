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
#include "defines.h"

void mainGame(DisplayDevice* DDevice, InputDevice* IDevice){
    /* Declaration */
    Characters* MainCharacter = NULL;
    CharacterLayer* CharaLayer = NULL;
    Map* WorldMap = NULL;
    bool DebugMode = false;
    int nbOfKeys;
    const Uint8* keyState;

    unsigned int IdleAnim = 0;
    bool IdleFlip = false;

    /* Init */
    InitDebug(DDevice);
    
    WorldMap = LoadMap(DDevice, "Assets/WorldMaps/OverWorld.txt");
    MainCharacter = InitCharacter(DDevice, "Assets/Characters/MainCharacter.xml");

    InitCharacterLayer(DDevice, &CharaLayer);
    AddCharacterToLayer(CharaLayer, MainCharacter, HERO_START_X, HERO_START_Y, false);

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

        keyState = SDL_GetKeyboardState(&nbOfKeys);
        if (keyState[SDL_SCANCODE_UP]){
            CharacterPlayAnimation(MainCharacter, 4, false); /* Up */
            setCharacterProperty(CharaLayer, 0, true, false);
            IdleAnim = 1;
            IdleFlip = false;
        } else if (keyState[SDL_SCANCODE_DOWN]){
            CharacterPlayAnimation(MainCharacter, 3, false); /* Down */
            setCharacterProperty(CharaLayer, 0, true, false);
            IdleAnim = 0;
            IdleFlip = false;
        } else if (keyState[SDL_SCANCODE_LEFT]){
            CharacterPlayAnimation(MainCharacter, 5, false); /* Left */
            setCharacterProperty(CharaLayer, 0, true, true);
            IdleAnim = 2;
            IdleFlip = true;
        } else if (keyState[SDL_SCANCODE_RIGHT]){
            CharacterPlayAnimation(MainCharacter, 5, false); /* Right */
            setCharacterProperty(CharaLayer, 0, true, false);
            IdleAnim = 2;
            IdleFlip = false;
        } else {
            CharacterPlayAnimation(MainCharacter, IdleAnim, false); /* Idle */
            setCharacterProperty(CharaLayer, 0, true, IdleFlip);
        }
        
        DisplayWorldMap(DDevice, WorldMap); /* Draw World Map */
        DisplayCharacterLayer(DDevice, CharaLayer);         /* Draw the main character */
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
