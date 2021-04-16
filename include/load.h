#ifndef _LOAD_H
#define _LOAD_H

    #include <SDL2/SDL.h>
    #include <SDL2/SDL_mixer.h>
    #include <types.h>

    SDL_Surface* LoadSDLSurface(char FilePath[], DisplayDevice* Device, Uint32* ColorKey);
    Surface* LoadSurface(char FilePath[], DisplayDevice* Device, Uint32* ColorKey, char AlphaChannel);
    BitmapFont* LoadBitmapFont(char FilePath[], DisplayDevice* DDevice, Uint32 FontColorKey);

    Surface* CreateTargetSurface(DisplayDevice* DDevice, int w, int h);

    Mix_Music* LoadMusic(char FilePath[]);
    Mix_Chunk* LoadSoundEffect(char FilePath[]);

#endif