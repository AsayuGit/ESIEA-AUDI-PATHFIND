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
    void CenterCameraOn(DisplayDevice* DDevice, Map* WorldMap, int x, int y);

#endif
