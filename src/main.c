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
#include "time.h"

enum {
    MainMode,
    DebugMode,
    AStarMode,
    VictoryMode
};  

void mainGame(DisplayDevice* DDevice, InputDevice* IDevice){
    /* Declaration */
    SDL_Texture* CongratsTexture = NULL;
    CharacterLayer* CharaLayer = NULL;
    CharacterList* CharaHandle = NULL;
    Characters* MainCharacter = NULL;
    Characters* Chest = NULL;
    Vector2iLinkedList* path = NULL;
    Vector2i PlayerMapCoordinates = {HERO_START_X, HERO_START_Y};
    Vector2i* potentialChest = NULL;
    Vector2i ChestArray[4];
    Uint32 oldTime, frametime, newTime = 0;
    Map* WorldMap = NULL;
    const Uint8* keyState;
    unsigned char EventMode = MainMode;
    unsigned char i;
    unsigned int IdleAnim = 0;
    unsigned int nbOfPotentialChests = 0;
    unsigned int nextChest;
    double PlayerMove = 0.0f;
    bool animSet = 0;
    int nbOfKeys;
    /* Init */
    srand(time(NULL));
    InitDebug(DDevice);
    InitCharacterLayer(DDevice, &CharaLayer);
    
    CongratsTexture = LoadSurface("Assets/Textures/Menus/Congrats.bmp", DDevice, 0xff00ff, SURFACE_KEYED);
    WorldMap = LoadMap(DDevice, "Assets/WorldMaps/OverWorld.txt");
    MainCharacter = InitCharacter(DDevice, "Assets/Characters/MainCharacter.xml");
    Chest = InitCharacter(DDevice, "Assets/Characters/Chest.xml");
    nbOfPotentialChests = FindPotentialChestLocations(WorldMap, &potentialChest);

    /* Chests generation */
    for (i = 0; i < 4; i++){
        nextChest = rand()%nbOfPotentialChests;
        AddCharacterToLayer(CharaLayer, Chest, potentialChest[nextChest].x * TILE_SIZE, potentialChest[nextChest].y * TILE_SIZE, false);
        ChestArray[i] = potentialChest[nextChest];
        /*printf("NYAN %d %d\n", ChestArray[i].x, ChestArray[i].y);*/
    }

    CharaHandle = AddCharacterToLayer(CharaLayer, MainCharacter, HERO_START_X * TILE_SIZE + (TILE_SIZE >> 1), HERO_START_Y  * TILE_SIZE + (TILE_SIZE >> 1), false);

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
                    EventMode = (EventMode == DebugMode) ? MainMode : DebugMode;
                    SetDebugCursorPos(PlayerMapCoordinates);
                    break;
                case SDL_SCANCODE_SPACE:
                    if (EventMode == MainMode){
                        printf("Path set !\n");
                        if (path){
                            FreeVector2iLinkedList(path);
                            path = NULL;
                        }
                        path = getPath(WorldMap, PlayerMapCoordinates, InitVector2i(16, 4));
                        setPath(path);
                        EventMode = AStarMode;
                    }
                    break;
                default:
                    break;
                }
            break;
            default:
                break;
            }
            if (EventMode == DebugMode)
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
            
            case VictoryMode: /* Fall through */
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
        

        /* Chest Colosions */
        for (i = 0; i < 4; i++){
            if ((ChestArray[i].x == PlayerMapCoordinates.x) && (ChestArray[i].y == PlayerMapCoordinates.y)){
                EventMode = VictoryMode;
                CharacterPlayAnimation(MainCharacter, IdleAnim, false);
                setCharacterProperty(CharaLayer, 0, true, CharaHandle->Flip);
                break;
            }
        }

        DisplayWorldMap(DDevice, WorldMap); /* Draw World Map */
        DisplayCharacterLayer(DDevice, CharaLayer);         /* Draw the main character */
        if (EventMode == DebugMode)
            DisplayMapEditor(DDevice);
        if (EventMode == VictoryMode)
            SDL_RenderCopy(DDevice->Renderer, CongratsTexture, NULL, NULL);
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
