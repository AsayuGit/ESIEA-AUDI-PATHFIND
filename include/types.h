#ifndef _TYPES_H
#define _TYPES_H

    #include <SDL2/SDL.h>
    #include <stdbool.h>

    typedef SDL_Texture Surface;
    #define SDL_HWSURFACE SDL_SWSURFACE

    typedef struct{
        int x;
        int y;
    } Vector2i;

    typedef struct{
        double x;
        double y;
    } Vector2d;

    typedef struct{
        char* TileMapPath;
        Surface* TileMapSurface;
        unsigned int MapSizeX;
        unsigned int MapSizeY;
        unsigned int MapSize;
        Uint32 ColorKey;
    } TileMap;

    typedef struct{
        unsigned int** MapData;
        unsigned int MapSizeX;
        unsigned int MapSizeY;
        SDL_Rect MapRegion;
        TileMap MapTileMap;
    } Map;

    typedef struct{
        SDL_Window *Screen;
        SDL_Renderer *Renderer;
        Vector2i ScreenResolution;
        SDL_Rect Camera;
    } DisplayDevice;

    typedef struct{
        /* Events */
        SDL_Event event;

        /* Key Presses */
        const Uint8* KeyStates; /* Pointer to the keypresses */
    } InputDevice;

    typedef struct{
        SDL_Surface* FontSurface;
        SDL_Texture* FontTexture;
        unsigned int FontHeight;
    } BitmapFont;

    Vector2i InitVector2i(int x, int y);
    SDL_Rect InitSDL_Rect(int x, int y, int w, int h);

#endif
