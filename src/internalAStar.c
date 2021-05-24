#include "internalAStar.h"

node closedset[MAX_NODES]; /* Nodes à exploré */
node openset[MAX_NODES]; /* Nodes déja exploré */
int closedset_size, openset_size;

/* le score f reprensente la distance entre un node et la cible. A* va explore le plus proche en premier. */
node* find_openset_lowest_f_score(){
	float minimum = openset[0].f;
	int lowest = 0;
	int i;
	for (i = 1; i < openset_size; i++){
		if (openset[i].f < minimum){
			minimum = openset[i].f;
			lowest = i;
		}
	}
	return &openset[lowest];
}

int is_in_closedset(node* n){
	int i;

	for (i = 0; i < closedset_size; i++){
		if (closedset[i].x == n->x && closedset[i].y == n->y)
			return 1;
	}
	return 0;
}

int is_in_openset(node* n){
	int i;

	for (i = 0; i < openset_size; i++){
		if (openset[i].x == n->x && openset[i].y == n->y)
			return 1;
	}
	return 0;
}

node* switch_to_closedset(node* n){
	int i;

	closedset[closedset_size++] = *n;
	for (i = 0; i < openset_size; i++){
		if (openset[i].x == n->x && openset[i].y == n->y){
			memmove(&openset[i], &openset[i + 1], sizeof(node)* (openset_size - i - 1));
			openset_size--;
			break;
		}
	}
	return &closedset[closedset_size - 1];
}

/* int astar(int* map, int map_width, int map_height, int debut_x, int debut_y, int fin_x, int fin_y, astar_result* result){*/
/* CORRECTION K.R. */
int astar(unsigned int** map, int map_width, int map_height, int debut_x, int debut_y, int fin_x, int fin_y, astar_result* result){
	int i;
	node tempNode, temp, voisin;

	result->pathsize = closedset_size = openset_size = 0;
	tempNode.x = debut_x;
	tempNode.y = debut_y;
	tempNode.f = fabs(debut_x - fin_x) + fabs(debut_y - fin_y);
	tempNode.g = 0;
	tempNode.h = 0;
	tempNode.previous = NULL;

	openset[openset_size++] = tempNode;
	while (openset_size > 0){
		node* current = find_openset_lowest_f_score();
		/* un cheminimumimum vers la cible à été trouvé */
		if (current->x == fin_x && current->y == fin_y){
			result->path[result->pathsize++] = *current;
			/* Refais le cheminimumimum trouvé */
			while (current->x != debut_x || current->y != debut_y){
				result->path[result->pathsize++] = *current = *current->previous;
			}
			/* inverse le cheminimum */
			for (i = 0; i < result->pathsize / 2; i++){
				temp = result->path[i];
				result->path[i] = result->path[result->pathsize - 1 - i];
				result->path[result->pathsize - 1 - i] = temp;
			}
			return 1;
		}
		current = switch_to_closedset(current);
		/* cherche les 4 directions du node actuel */
		for (i = 0; i < 4; i++){
			voisin.x = current->x;
			voisin.y = current->y;
			voisin.f = 0;
			voisin.g = 0;
			voisin.h = 0;
			voisin.previous = NULL;
			switch (i){
				case 0: voisin.y--; break;
				case 1: voisin.x++; break;
				case 2: voisin.y++; break;
				case 3: voisin.x--; break;
			}
			/* ignore les cellules déja exploré, les cellules hors map et celle impassable */
			/*if (is_in_closedset(&voisin) || voisin.x < 0 || voisin.y < 0 || voisin.x >= map_width || voisin.y >= map_height || map[voisin.x * map_height + voisin.y] > 0)*/
			/* CORRECTION K.R. */
			if (is_in_closedset(&voisin) || voisin.x < 0 || voisin.y < 0 || voisin.x >= map_width || voisin.y >= map_height || map[voisin.y][voisin.x] > 0)
				continue;
			if (!is_in_openset(&voisin) || current->g + 1 < voisin.g){
				voisin.previous = current;
				voisin.g = current->g + 1;
				voisin.f = voisin.g + abs(voisin.x - fin_x) + abs(voisin.y - fin_y);
				if (!is_in_openset(&voisin))
					openset[openset_size++] = voisin;
			}
		}
	}
	/* pas de cheminimum trouvé */
	return 0;
}