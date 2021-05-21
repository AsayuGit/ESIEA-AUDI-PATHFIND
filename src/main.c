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
    CharacterList* CharaHandle = NULL;
    CharacterLayer* CharaLayer = NULL;
    Map* WorldMap = NULL;
    bool DebugMode = false, animSet = 0;
    int nbOfKeys;
    const Uint8* keyState;
    unsigned int IdleAnim = 0;
    Uint32 oldTime, frametime, newTime = 0;
    Vector2i PlayerMapCoordinates = {0, 0};
    double PlayerMove = 0.0f;

    /* Init */
    InitDebug(DDevice);
    
    WorldMap = LoadMap(DDevice, "Assets/WorldMaps/OverWorld.txt");
    MainCharacter = InitCharacter(DDevice, "Assets/Characters/MainCharacter.xml");

    InitCharacterLayer(DDevice, &CharaLayer);
    CharaHandle = AddCharacterToLayer(CharaLayer, MainCharacter, HERO_START_X, HERO_START_Y, false);

    /* Main Game loop */
    while (true){
        oldTime = newTime;
        newTime = SDL_GetTicks();
        frametime = newTime - oldTime;
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
                    SetDebugCursorPos(PlayerMapCoordinates);
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

        animSet = false;
        keyState = SDL_GetKeyboardState(&nbOfKeys);
        if (!DebugMode){
            PlayerMove = HERO_SPEED * (frametime / SPEED_SCALE);

            if (keyState[SDL_SCANCODE_UP]){
                if (!animSet){
                    CharacterPlayAnimation(MainCharacter, 4, false); 
                    animSet = true;
                    CharaHandle->Flip = false;
                    IdleAnim = 1;
                }
                nextValidPosition(&CharaHandle->Coordinates, WorldMap, 0, -PlayerMove);
            } else if (keyState[SDL_SCANCODE_DOWN]){
                if (!animSet){
                    CharacterPlayAnimation(MainCharacter, 3, false); /* Down */
                    animSet = true;
                    CharaHandle->Flip = false;
                    IdleAnim = 0;
                }
                nextValidPosition(&CharaHandle->Coordinates, WorldMap, 0, PlayerMove);
            } 
            
            if (keyState[SDL_SCANCODE_LEFT]){
                if (!animSet){
                    CharacterPlayAnimation(MainCharacter, 5, false); /* Left */
                    animSet = true;
                    CharaHandle->Flip = true;
                    IdleAnim = 2;
                }
                nextValidPosition(&CharaHandle->Coordinates, WorldMap, -PlayerMove, 0);
            } else if (keyState[SDL_SCANCODE_RIGHT]){
                if (!animSet){
                    CharacterPlayAnimation(MainCharacter, 5, false); /* Right */
                    animSet = true;
                    CharaHandle->Flip = false;
                    IdleAnim = 2;
                }
                nextValidPosition(&CharaHandle->Coordinates, WorldMap, +PlayerMove, 0);
            } 
            
            if (!animSet){
                CharacterPlayAnimation(MainCharacter, IdleAnim, false); /* Idle */
                setCharacterProperty(CharaLayer, 0, true, CharaHandle->Flip);
            }
            CenterCameraOnPlayer(DDevice, WorldMap, CharaHandle->Coordinates);
        }

        PlayerMapCoordinates.x = (int)(CharaHandle->Coordinates.x / TILE_SIZE);
        PlayerMapCoordinates.y = (int)(CharaHandle->Coordinates.y / TILE_SIZE);
        
        if (WorldMap->MapData[PlayerMapCoordinates.y][PlayerMapCoordinates.x] == CHESTID){
            printf("Contgrats\n");
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
