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
#include "Jukebox.h"

enum {
    MainMode,
    DebugMode,
    AStarMode,
    VictoryMode
};  

void mainGame(DisplayDevice* DDevice, InputDevice* IDevice){
    /* Declaration */
    SDL_Texture* CongratsTexture = NULL;
    SDL_Rect CongratsTextureDstRect;
    Mix_Chunk* Yeah = NULL;

    CharacterLayer* CharaLayer = NULL;
    CharacterLayer* ChestLayer = NULL;
    CharacterList* CharaHandle = NULL;
    CharacterList* ChestHandle[4] = {NULL};
    Characters* MainCharacter = NULL;
    Characters* Chest = NULL;
    Vector2iLinkedList* pathArray[4] = {NULL};
    Vector2i PlayerMapCoordinates = {HERO_START_X, HERO_START_Y};
    Vector2i* potentialChest = NULL;
    Vector2i ChestArray[4];
    Uint32 oldTime, frametime, newTime = 0;
    Map* WorldMap = NULL;
    const Uint8* keyState;
    unsigned char EventMode, validChest, tempChest;
    unsigned int IdleAnim = 0;
    unsigned int nbOfPotentialChests = 0;
    unsigned int nextChest[4];
    double PlayerMove = 0.0f;
    bool animSet = 0;
    int nbOfKeys, i, j;
    /* Init */
    srand(time(NULL));
    InitDebug(DDevice);
    InitCharacterLayer(DDevice, &CharaLayer);
    InitCharacterLayer(DDevice, &ChestLayer);
    
    CongratsTexture = LoadSurface("Assets/Textures/Menus/Congrats.bmp", DDevice, 0xff00ff, SURFACE_KEYED);
    SDL_QueryTexture(CongratsTexture, NULL, NULL, &CongratsTextureDstRect.w, &CongratsTextureDstRect.h);

    ScaleToBiggestRectMultiple(&DDevice->Camera, &CongratsTextureDstRect);
    CongratsTextureDstRect.x = (DDevice->Camera.w - CongratsTextureDstRect.w) >> 1;
    CongratsTextureDstRect.y = (DDevice->Camera.h - CongratsTextureDstRect.h) >> 1;

    WorldMap = LoadMap(DDevice, "Assets/WorldMaps/OverWorld.txt");
    MainCharacter = InitCharacter(DDevice, "Assets/Characters/MainCharacter.xml");
    Chest = InitCharacter(DDevice, "Assets/Characters/Chest.xml");
    CharaHandle = AddCharacterToLayer(CharaLayer, MainCharacter, 0, 0, false);

    Yeah = Mix_LoadWAV(EffectPath[CHK_YEAH]);

    Mix_VolumeMusic(32);

BEGIN:
    PlayTrackID(TRK_BGM);
    Mix_FadeOutChannel(-1, 500);
    CharaHandle->Coordinates = InitVector2d(HERO_START_X * TILE_SIZE + (TILE_SIZE >> 1), HERO_START_Y  * TILE_SIZE + (TILE_SIZE >> 1));
    FreeCharacterList(ChestLayer->CharaList);
    ChestLayer->CharaList = NULL;
    nbOfPotentialChests = FindPotentialChestLocations(WorldMap, &potentialChest);
    /* Chests generation */
    validChest = rand()%4;

    nextChest[0] = rand()%nbOfPotentialChests;
    for (i = 1; i < 4; i++){
        
        /* générer un nouveau nombre aléatoire unique */
        tempChest = rand()%nbOfPotentialChests;
        for (j = 0; j < i; j++){
            if (tempChest == nextChest[j]){
                j = -1;
                tempChest = rand()%nbOfPotentialChests;
            }
        }
        
        nextChest[i] = tempChest;
    }

    for (i = 0; i < 4; i++){
        ChestHandle[i] = AddCharacterToLayer(ChestLayer, Chest, potentialChest[nextChest[i]].x * TILE_SIZE, potentialChest[nextChest[i]].y * TILE_SIZE, false);
        ChestArray[i] = potentialChest[nextChest[i]];
    }
    EventMode = MainMode;

    /* Main Game loop */
    while (true){
        oldTime = newTime;
        newTime = SDL_GetTicks();
        frametime = newTime - oldTime;
        /* Events Loop */
        while(SDL_PollEvent(&IDevice->event)){
            SystemEvents(DDevice, IDevice);

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
                    switch (EventMode)
                    {
                    case MainMode:
                        printf("Path set !\n");
                        for (i = 0; i < 4; i++){
                            if (pathArray[i]){
                                FreeVector2iLinkedList(pathArray[i]);
                                pathArray[i] = NULL;
                            }
                            if (ChestHandle[i]->PlayingAnimation == 0)
                                pathArray[i] = getPath(WorldMap, PlayerMapCoordinates, ChestArray[i]);
                        }

                        setPath(GetShortestEuclidianPath(pathArray, 4));
                        EventMode = AStarMode;
                        break;
                    
                    case VictoryMode:
                        goto BEGIN;
                        break;
                    
                    default:
                        break;
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
                        CharacterPlayAnimation(CharaHandle, 4, false); /* Up */
                        animSet = true;
                        CharaHandle->Flip = false;
                        IdleAnim = 1;
                    }
                    nextValidPosition(&CharaHandle->Coordinates, WorldMap, 0, -PlayerMove);
                } else if (keyState[SDL_SCANCODE_DOWN]){
                    if (!animSet){
                        CharacterPlayAnimation(CharaHandle, 3, false); /* Down */
                        animSet = true;
                        CharaHandle->Flip = false;
                        IdleAnim = 0;
                    }
                    nextValidPosition(&CharaHandle->Coordinates, WorldMap, 0, PlayerMove);
                } 
                
                if (keyState[SDL_SCANCODE_LEFT]){
                    if (!animSet){
                        CharacterPlayAnimation(CharaHandle, 5, false); /* Left */
                        animSet = true;
                        CharaHandle->Flip = true;
                        IdleAnim = 2;
                    }
                    nextValidPosition(&CharaHandle->Coordinates, WorldMap, -PlayerMove, 0);
                } else if (keyState[SDL_SCANCODE_RIGHT]){
                    if (!animSet){
                        CharacterPlayAnimation(CharaHandle, 5, false); /* Right */
                        animSet = true;
                        CharaHandle->Flip = false;
                        IdleAnim = 2;
                    }
                    nextValidPosition(&CharaHandle->Coordinates, WorldMap, +PlayerMove, 0);
                } 
            
            case VictoryMode: /* Fall through */
                if (!animSet){
                    CharacterPlayAnimation(CharaHandle, IdleAnim, false);
                    setCharacterProperty(CharaLayer, 0, true, CharaHandle->Flip);
                }
                CenterCameraOnPlayer(DDevice, WorldMap, CharaHandle->Coordinates);
                break;
            
            case AStarMode:
                if (WallOnPath(CharaHandle, PlayerMove, &IdleAnim))
                    EventMode = MainMode;
                CenterCameraOnPlayer(DDevice, WorldMap, CharaHandle->Coordinates);
                break;
            

        }

        PlayerMapCoordinates.x = (int)(CharaHandle->Coordinates.x / TILE_SIZE);
        PlayerMapCoordinates.y = (int)(CharaHandle->Coordinates.y / TILE_SIZE);
        

        /* Chest Colosions */
        if (EventMode == MainMode){
            for (i = 0; i < 4; i++){
                if ((ChestArray[i].x == PlayerMapCoordinates.x) && (ChestArray[i].y == PlayerMapCoordinates.y)){
                    CharacterPlayAnimation(ChestHandle[i], 1, true);
                    if (i == validChest){
                        StopTrack();
                        Mix_PlayChannel(-1, Yeah, 0);
                        EventMode = VictoryMode;
                        CharacterPlayAnimation(CharaHandle, IdleAnim, false);
                        setCharacterProperty(CharaLayer, 0, true, CharaHandle->Flip);
                    }
                    break;
                }
            }
        }

        MusicDaemon();

        DisplayWorldMap(DDevice, WorldMap);                 /* Draw World Map */
        DisplayCharacterLayer(DDevice, ChestLayer);         /* Draw the chests */
        DisplayCharacterLayer(DDevice, CharaLayer);         /* Draw the main character */
        if (EventMode == DebugMode)
            DisplayMapEditor(DDevice);
        if (EventMode == VictoryMode)
            ScaledDraw(DDevice, CongratsTexture, NULL, &CongratsTextureDstRect);
        FinishFrame(DDevice);
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
    DDevice = CreateDisplayDevice(640*2, 480*2, "Audi RO");
    IDevice = InitInputs(false);
    CreateSoundDevice();
    
    /* Game */
    mainGame(DDevice, IDevice);

    /* Free */
    free(IDevice);
    free(DDevice);
    
    return 0;
}
