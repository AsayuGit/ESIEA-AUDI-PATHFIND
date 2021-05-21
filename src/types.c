#include "types.h"

Vector2i InitVector2i(int x, int y){
    Vector2i Return;

    Return.x = x;
    Return.y = y;

    return Return;
}

Vector2d InitVector2d(double x, double y){
    Vector2d Return;

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

void AddToIntLinkedList(IntLinkedList** List, int data){
    if (!List)
        return;

    while (*List){
        List = (IntLinkedList**)&(*List)->next;
    }
    (*List) = (IntLinkedList*)malloc(sizeof(IntLinkedList));
    (*List)->data = data;
    (*List)->next = NULL;
}


void AddToVector2iLinkedList(Vector2iLinkedList** List, Vector2i data){
    if (!List)
        return;

    while (*List){
        List = (Vector2iLinkedList**)&(*List)->next;
    }
    (*List) = (Vector2iLinkedList*)malloc(sizeof(Vector2iLinkedList));
    (*List)->data = data;
    (*List)->next = NULL;
}

void FreeVector2iLinkedList(Vector2iLinkedList* List){
    if (List){
        FreeVector2iLinkedList((Vector2iLinkedList*)List->next);
        free(List);
    }
}