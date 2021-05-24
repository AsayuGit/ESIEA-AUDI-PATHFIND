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

#ifndef _CHARACTERS_H
#define _CHARACTERS_H

    #include "CharactersStructures.h"

    Characters* InitCharacter(DisplayDevice* DDevice, char* CharacterPath);
    void CharacterPlayAnimation(CharacterList* CharaList, int AnimationID, bool Restart);

    /* Character Layer Management */
    void FreeCharacterList(CharacterList* CharaList);
    void FreeCharacterLayer(CharacterLayer* CharaLayer);
    void InitCharacterLayer(DisplayDevice* DDevice, CharacterLayer** CharaLayer);
    CharacterList* AddCharacterToLayer(CharacterLayer* CharaLayer, Characters* Character, double X, double Y, bool Flip);
    void removeCharacterFromLayer(CharacterLayer* CharaLayer, const unsigned int charaInLayerID);
    void setCharacterProperty(CharacterLayer* CharaLayer, const unsigned int charaInLayerID, bool Shown, bool Flipped);
    
    /* Display */
    void DisplayCharacterLayer(DisplayDevice* DDevice, CharacterLayer* CharaLayer);

#endif