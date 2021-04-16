#include "load.h"

#ifdef _SDL
    SDL_Surface* UseAlphaChannel(SDL_Surface* AlphaSurface){
        SDL_SetAlpha(AlphaSurface, SDL_SRCALPHA, SDL_ALPHA_OPAQUE);
        AlphaSurface->format->Amask = 0xFF000000;
        //AlphaSurface->format->Ashift = 24;
        return AlphaSurface;
    }
#endif

SDL_Surface* LoadSDLSurface(char FilePath[], DisplayDevice* Device, Uint32* ColorKey){
    SDL_Surface* LoadingSurface;
    
    #ifdef _SDL
        SDL_Surface* AcceleratedSurface;
    #endif

    LoadingSurface = SDL_LoadBMP(FilePath);
    #ifdef _SDL
        AcceleratedSurface = SDL_DisplayFormat(LoadingSurface);
        SDL_FreeSurface(LoadingSurface);
        if (ColorKey != NULL){
            SDL_SetColorKey(AcceleratedSurface, SDL_SRCCOLORKEY | SDL_RLEACCEL, *ColorKey);
        }else{
            
        }
        return AcceleratedSurface;
    #else
        if (ColorKey != NULL)
            SDL_SetColorKey(LoadingSurface, true, *ColorKey);
        return LoadingSurface;
    #endif
}

/* FIXME: ColorKey shouldn't be a pointer, ideally we would want a flag system to state how we want to load a said texture */
Surface* LoadSurface(char FilePath[], DisplayDevice* Device, Uint32* ColorKey, char AlphaChannel){
    SDL_Surface* loadingSurface;
    
    if (!FilePath) /* Don't bother loading a surface if the path isn't provided */
        return NULL;
    loadingSurface = LoadSDLSurface(FilePath, Device, ColorKey);
    if (loadingSurface){
        #ifdef _SDL
            return (AlphaChannel) ? UseAlphaChannel(loadingSurface) : loadingSurface;
        #else
            return SDL_CreateTextureFromSurface(Device->Renderer, loadingSurface);
        #endif
    } else {
        printf("ERROR! : Couldn't load %s !\n", FilePath);
    }
    return NULL;
}

/*
BitmapFont* LoadBitmapFont(char FilePath[], DisplayDevice* DDevice, Uint32 FontColorKey){
    BitmapFont* LoadingFont;

    LoadingFont = (BitmapFont*)malloc(sizeof(BitmapFont));
    LoadingFont->FontSurface = NULL;
    LoadingFont->FontSurface = LoadSDLSurface(FilePath, DDevice, &FontColorKey);
    if (LoadingFont->FontSurface == NULL){
        fprintf(stderr, "Can't load font %s\n", SDL_GetError());
    }
    #ifndef _SDL
        LoadingFont->FontTexture = SDL_CreateTextureFromSurface(DDevice->Renderer, LoadingFont->FontSurface);
    #endif
    LoadingFont->FontHeight = getFontHeight(LoadingFont);

    return LoadingFont;
}*/

Surface* CreateTargetSurface(DisplayDevice* DDevice, int w, int h){
    Surface* LoadingSurface;

    LoadingSurface = NULL;
#ifdef _SDL
    LoadingSurface = CreateEmptySurface(w, h);
#else
    LoadingSurface = SDL_CreateTexture(DDevice->Renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetTextureBlendMode(LoadingSurface, SDL_BLENDMODE_BLEND);
#endif
    if (LoadingSurface == NULL)
        fprintf(stderr, "Can't load texture %s\n", SDL_GetError());
    return LoadingSurface;
}

Mix_Music* LoadMusic(char FilePath[]){
    Mix_Music* LoadingMusic;
    
    LoadingMusic = NULL;
    LoadingMusic = Mix_LoadMUS(FilePath);
    
    if (LoadingMusic == NULL)
        fprintf(stderr, "Can't load music %s\n", Mix_GetError());
    return LoadingMusic;
}

Mix_Chunk* LoadSoundEffect(char FilePath[]){
    Mix_Chunk* LoadingSoundEffect;
    
    LoadingSoundEffect = NULL;
    LoadingSoundEffect = Mix_LoadWAV(FilePath);

    if (LoadingSoundEffect == NULL)
        fprintf(stderr, "Can't load sound effect %s\n", Mix_GetError());
    return LoadingSoundEffect;
}