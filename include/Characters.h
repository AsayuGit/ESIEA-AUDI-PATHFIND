#ifndef _CHARACTERS_H
#define _CHARACTERS_H

    #include "CharactersStructures.h"

    Characters* InitCharacter(DisplayDevice* DDevice, char* CharacterPath);
    void CharacterPlayAnimation(Characters* Character, int AnimationID);

    /* Character Layer Management */
    void InitCharacterLayer(DisplayDevice* DDevice, CharacterLayer** CharaLayer);
    void AddCharacterToLayer(CharacterLayer* CharaLayer, Characters* Character, int TileID, bool Flip);
    void removeCharacterFromLayer(CharacterLayer* CharaLayer, const unsigned int charaInLayerID);
    void setCharacterVisibility(CharacterLayer* CharaLayer, const unsigned int charaInLayerID, bool Shown);
    
    /* Display */
    void DisplayCharacterLayer(DisplayDevice* DDevice, CharacterLayer* CharaLayer);

#endif