#ifndef _DEBUG_H
#define _DEBUG_H

    #include "types.h"

    void InitDebug(DisplayDevice* DDevice);
    void DebugEvents(DisplayDevice* DDevice, InputDevice* IDevice, Map* WorldMap);
    void DisplayMapEditor(DisplayDevice* DDevice);

#endif