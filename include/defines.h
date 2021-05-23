#ifndef _DEFINES_H
#define _DEFINES_H

    #define TILE_SIZE 32
    #define HERO_START_X 5
    #define HERO_START_Y 2
    #define HERO_SPEED 2.5f
    #define SPEED_SCALE (1000.0f / TILE_SIZE)
    #define CHESTID 0
    #ifndef PATH_MAX
        #define PATH_MAX 100
    #endif

    #define MAX(x, y) (((x) > (y)) ? (x) : (y))
    #define MIN(x, y) (((x) < (y)) ? (x) : (y))

#endif