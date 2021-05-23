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

#ifndef _KEYMAP_H
#define _KEYMAP_H

    #ifdef _SDL
        /* Actions */
        #define PAD_SELECT SDLK_SPACE
        #define PAD_BACK SDLK_LCTRL
        #define PAD_COURTRECORD SDLK_E

        /* Directions */
        #define PAD_UP SDLK_UP
        #define PAD_DOWN SDLK_DOWN
        #define PAD_LEFT SDLK_LEFT
        #define PAD_RIGHT SDLK_RIGHT

        #define PADKEY key.keysym.sym
    #else
        /* Actions */
        #define PAD_SELECT SDL_SCANCODE_SPACE
        #define PAD_BACK SDL_SCANCODE_LCTRL

        /* Directions */
        #define PAD_UP SDL_SCANCODE_UP
        #define PAD_DOWN SDL_SCANCODE_DOWN
        #define PAD_LEFT SDL_SCANCODE_LEFT
        #define PAD_RIGHT SDL_SCANCODE_RIGHT

        #define PAD_PLUS SDL_SCANCODE_KP_PLUS
        #define PAD_MINUS SDL_SCANCODE_KP_MINUS

        #define PADKEY key.keysym.scancode
        #define MICEKEY button.button
    #endif

#endif