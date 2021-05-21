#ifndef _CHARACTERS_H
#define _CHARACTERS_H

    #include "CharactersStructures.h"

    Characters* InitCharacter(DisplayDevice* DDevice, char* CharacterPath);
    void CharacterPlayAnimation(Characters* Character, int AnimationID, bool Restart);

    /* Character Layer Management */
    void InitCharacterLayer(DisplayDevice* DDevice, CharacterLayer** CharaLayer);
    void AddCharacterToLayer(CharacterLayer* CharaLayer, Characters* Character, unsigned int X, unsigned int Y, bool Flip);
    void removeCharacterFromLayer(CharacterLayer* CharaLayer, const unsigned int charaInLayerID);
    void setCharacterProperty(CharacterLayer* CharaLayer, const unsigned int charaInLayerID, bool Shown, bool Flipped);
    
    /* Display */
    void DisplayCharacterLayer(DisplayDevice* DDevice, CharacterLayer* CharaLayer);

#endif