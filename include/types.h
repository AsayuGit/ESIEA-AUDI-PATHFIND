#ifndef _TYPES_H
#define _TYPES_H

    #include <SDL2/SDL.h>
    #include <stdbool.h>

    typedef SDL_Texture Surface;
    #define SDL_HWSURFACE SDL_SWSURFACE
    #define PADKEY event.key.keysym.scancode
    #define P_PADKEY event->key.keysym.scancode

    typedef struct{
        int x;
        int y;
    } Vector2i;

    typedef struct{
        double x;
        double y;
    } Vector2d;

    typedef struct{
        Surface* TileMapSurface;
        unsigned int MapSizeX;
    } TileMap;

    typedef struct{
        unsigned int** MapData;
        unsigned int MapSizeX;
        unsigned int MapSizeY;
        TileMap MapTileMap;
    } Map;

    typedef struct{
        SDL_Window *Screen;
        SDL_Renderer *Renderer;
        Vector2i ScreenResolution;
    } DisplayDevice;

    typedef struct{
        /* Events */
        SDL_Event event;
        bool EventEnabled;

        /* Key Presses */
        const Uint8* KeyStates; /* Pointer to the keypresses */
        
        /* Joystick */
        SDL_Joystick* Joy1; /* Pointers to the Joypad */
        bool JoyEnabled;
    } InputDevice;

    typedef struct{
        SDL_Surface* FontSurface;
        SDL_Texture* FontTexture;
        unsigned int FontHeight;
    } BitmapFont;

#endif
