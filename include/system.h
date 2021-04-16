#ifndef _SYSTEM_H
#define _SYSTEM_H

    #include "system.h"
    #include "types.h"

    void InitSDL();
    InputDevice* InitInputs(bool JoyEnabled);
    DisplayDevice* CreateDisplayDevice(int ScreenWidth, int ScreenHeight, char* Title);

    void CreateSoundDevice();

#endif
