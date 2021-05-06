#include "types.h"

Vector2i InitVector2i(int x, int y){
    Vector2i Return;

    Return.x = x;
    Return.y = y;

    return Return;
}

SDL_Rect InitSDL_Rect(int x, int y, int w, int h){
    SDL_Rect Return;

    Return.x = x;
    Return.y = y;
    Return.w = w;
    Return.h = h;

    return Return;
}