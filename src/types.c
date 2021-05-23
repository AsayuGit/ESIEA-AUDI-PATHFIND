/*
    PATHFIND is a simple game where the player aims to find chests either
    by themself or by using the included pathfinding algorythm

    Copyright (C) 2021 Killian RAIMBAUD (killian.rai@gmail.com)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; version 2 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "types.h"

SDL_Rect InitRect(int x, int y, int w, int h){
    SDL_Rect rect;
    
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    return rect;
}


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