#include "aStar.h"

/* Placeholder function */
Vector2iLinkedList* getPath(Map* WorldMap, Vector2i TileStart, Vector2i TileEnd){
    Vector2iLinkedList* returnPath = NULL;

    AddToVector2iLinkedList(&returnPath, TileStart);
    AddToVector2iLinkedList(&returnPath, TileEnd);

    return returnPath;
}