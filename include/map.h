#ifndef _MAP_H
#define _MAP_H

    #include "types.h"

    Map* LoadMap(DisplayDevice* DDevice, char* MapFilePath);
    void SaveMap(Map* MapToSave, char* MapFilePath);
    
#endif