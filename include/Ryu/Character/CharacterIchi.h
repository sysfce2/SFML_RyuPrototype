#pragma once

#include <Ryu/Character/CharacterBase.h>
#include <Ryu/Core/AssetIdentifiers.h>

namespace ryu {

typedef AssetManager<sf::Texture, Textures::CharacterID> IchiTextureManager;

class CharacterIchi : public CharacterBase
{
    public:
        explicit CharacterIchi(ECharacterState startState);
        void setTextureOnCharacter(Textures::CharacterID textureId) override;
    
        //TODO: make it private and load otherwise ? / atm. public for Game when creating the mPlayer in Cdor
        void loadTextures();

    private:        
        IchiTextureManager ichiTextureManager;
};

} /// namespace ryu