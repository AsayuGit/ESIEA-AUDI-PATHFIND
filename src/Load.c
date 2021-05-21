#include "Load.h"

SDL_Surface* LoadSDLSurface(char FilePath[], DisplayDevice* Device){
    SDL_Surface* LoadingSurface;

    LoadingSurface = SDL_LoadBMP(FilePath);

    return LoadingSurface;
}

void KeySurface(SDL_Surface* SurfaceToKey, Uint32 ColorKey){
    SDL_SetColorKey(SurfaceToKey, true, ColorKey);
}

SDL_Texture* LoadSurface(char FilePath[], DisplayDevice* Device, Uint32 ColorKey, char flags){
    SDL_Surface* loadingSurface;
    
    if (!FilePath) /* Don't bother loading a surface if the path isn't provided */
        return NULL;
    loadingSurface = LoadSDLSurface(FilePath, Device);
    if (loadingSurface){

        if (SURFACE_KEYED & flags){
            KeySurface(loadingSurface, ColorKey);
        }

        return SDL_CreateTextureFromSurface(Device->Renderer, loadingSurface);

    } else {
        printf("ERROR: (loadSurface) Couldn't load %s !\n", FilePath);
    }
    return NULL;
}

SDL_Texture* CreateTargetSurface(DisplayDevice* DDevice, int w, int h){
    SDL_Texture* LoadingSurface;

    LoadingSurface = NULL;

    LoadingSurface = SDL_CreateTexture(DDevice->Renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetTextureBlendMode(LoadingSurface, SDL_BLENDMODE_BLEND);

    if (LoadingSurface == NULL)
        fprintf(stderr, "Can't load texture %s\n", SDL_GetError());
    return LoadingSurface;
}

Mix_Music* LoadMusic(char FilePath[]){
    Mix_Music* LoadingMusic = NULL;

    if (FilePath){
        LoadingMusic = Mix_LoadMUS(FilePath);
        if (LoadingMusic == NULL)
            fprintf(stderr, "Can't load music %s\n", Mix_GetError());
    }
    
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

xmlDoc* loadXml(char* filePath){
    xmlKeepBlanksDefault(0); /* Ignore white space */

	#ifdef _XBOX
		return xmlParseFile(filePath); /* Load File into memory */
	#else
		return xmlReadFile(filePath, NULL, 0); /* Load File into memory */
	#endif
}