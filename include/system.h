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

#ifndef _SYSTEM_H
#define _SYSTEM_H

    #include "system.h"
    #include "types.h"

    void InitSDL();
    InputDevice* InitInputs(bool JoyEnabled);
    DisplayDevice* CreateDisplayDevice(int ScreenWidth, int ScreenHeight, char* Title);

    void CreateSoundDevice();
    void BoundCameraToRegion(DisplayDevice* DDevice, SDL_Rect Region);
    void BoundVect2dToRegion(Vector2d* Vect, SDL_Rect Region);
    Vector2i GetCameraPos(DisplayDevice* DDevice);
    void CenterCameraOnTile(DisplayDevice* DDevice, Map* WorldMap, int x, int y);
    void CenterCameraOnPlayer(DisplayDevice* DDevice, Map* WorldMap, Vector2d PlayerPosition);
    char*    astrcpy(char** dst, char* src);
    int DrawEx(DisplayDevice* DDevice, SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect, bool flip);
    int Draw(DisplayDevice* DDevice, SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect);
    int ScaledDrawEx(DisplayDevice* DDevice, SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect, bool flip);
    int ScaledDraw(DisplayDevice* DDevice, SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect);
    void SystemEvents(DisplayDevice* DDevice, InputDevice* IDevice);
    void FinishFrame(DisplayDevice* DDevice);
    
#endif
