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
#include "aStar.h"

enum {
    MainMode,
    DebugMode,
    AStarMode
};  

void mainGame(DisplayDevice* DDevice, InputDevice* IDevice){
    /* Declaration */
    Characters* MainCharacter = NULL;
    CharacterList* CharaHandle = NULL;
    CharacterLayer* CharaLayer = NULL;
    Map* WorldMap = NULL;
    bool animSet = 0;
    int nbOfKeys;
    const Uint8* keyState;
    unsigned int IdleAnim = 0;
    Uint32 oldTime, frametime, newTime = 0;
    Vector2i PlayerMapCoordinates = {0, 0};
    double PlayerMove = 0.0f;
    Vector2iLinkedList* path = NULL;
    unsigned char EventMode = MainMode;

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
                    EventMode = (EventMode = DebugMode) ? MainMode : DebugMode;
                    SetDebugCursorPos(PlayerMapCoordinates);
                    break;
                case SDL_SCANCODE_SPACE:
                    printf("Path set !\n");
                    if (path){
                        FreeVector2iLinkedList(path);
                        path = NULL;
                    }
                    path = getPath(WorldMap, InitVector2i(HERO_START_X, HERO_START_Y), InitVector2i(16, 4));
                    setPath(path);
                    EventMode = AStarMode;
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
        switch (EventMode) {
            case MainMode:
                PlayerMove = HERO_SPEED * (frametime / SPEED_SCALE);

                if (keyState[SDL_SCANCODE_UP]){
                    if (!animSet){
                        CharacterPlayAnimation(MainCharacter, 4, false); /* Up */
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
                    CharacterPlayAnimation(MainCharacter, IdleAnim, false);
                    setCharacterProperty(CharaLayer, 0, true, CharaHandle->Flip);
                }
                CenterCameraOnPlayer(DDevice, WorldMap, CharaHandle->Coordinates);
                break;
            case AStarMode:
                if (WallOnPath(&CharaHandle->Coordinates, PlayerMove, CharaHandle))
                    EventMode = MainMode;
                CenterCameraOnPlayer(DDevice, WorldMap, CharaHandle->Coordinates);
                break;
            

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
