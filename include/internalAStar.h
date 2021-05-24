#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef _INTERNALASTAR_H
#define _INTERNALASTAR_H

/* nombre maximum de node analysé lors de l'exécution si depasse prob/crash et il faut alors l'augmente */
#define MAX_NODES 1000
/* un node est une tile avec les coordonne (x,y) */
typedef struct node node;
struct node{
	int x, y; /* Node coordinates */
	float f, g, h; /* variable interne pour le A* */
	node* previous; /* node précedent */
};

typedef struct astar_result{
	node path[MAX_NODES];
	int pathsize;
}astar_result;


/* Calculate the path between start and goal. */
/* map : pointer to a 2D int array (you can change int for short, char or other to suit your needs). 0 : walkable, anything > 0 : unwalkable. See astar.c line 82 (last condition) to change which values are considered walkable or not. */
/* result : pointer to a structure that holds the results of the pathfinding. Read result->path[i] (i from 0 to result->pathsize-1) to get your path. */
/* Returns 1 if a path was found, 0 if not. */
int astar(unsigned int** map, int map_width, int map_height, int debut_x, int debut_y, int fin_x, int fin_y, astar_result* result);


#endif