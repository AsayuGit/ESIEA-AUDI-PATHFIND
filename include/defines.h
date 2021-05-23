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

#ifndef _DEFINES_H
#define _DEFINES_H

    #define BASE_RESOLUTION_X 640
    #define BASE_RESOLUTION_Y 480
    #define TILE_SIZE 32
    #define HERO_START_X 5
    #define HERO_START_Y 2
    #define HERO_SPEED 2.5f
    #define SPEED_SCALE (1000.0f / TILE_SIZE)
    #define CHESTID 0
    #ifndef PATH_MAX
        #define PATH_MAX 100
    #endif

    #define MAX(x, y) (((x) > (y)) ? (x) : (y))
    #define MIN(x, y) (((x) < (y)) ? (x) : (y))

#endif