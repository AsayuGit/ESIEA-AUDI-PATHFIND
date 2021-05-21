#ifndef _SYSTEM_H
#define _SYSTEM_H

    #include "system.h"
    #include "types.h"

    void InitSDL();
    InputDevice* InitInputs(bool JoyEnabled);
    DisplayDevice* CreateDisplayDevice(int ScreenWidth, int ScreenHeight, char* Title);

    void CreateSoundDevice();
    void BoundCameraToRegion(DisplayDevice* DDevice, SDL_Rect Region);
    Vector2i GetCameraPos(DisplayDevice* DDevice);
    void CenterCameraOnTile(DisplayDevice* DDevice, Map* WorldMap, int x, int y);
    void CenterCameraOnPlayer(DisplayDevice* DDevice, Map* WorldMap, Vector2d PlayerPosition);
    char*    astrcpy(char** dst, char* src);
    
#endif
