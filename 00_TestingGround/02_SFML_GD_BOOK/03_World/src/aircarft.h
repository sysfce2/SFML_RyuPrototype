#pragma once

#include "entity.h"
#include "assetmanager.h"

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;

class Aircraft : public Entity
{
    public:
        enum class Type
        {
            Eagle,
            Raptor
        };

    public:
        explicit Aircraft(Type type, const TextureHolder& textures);
        void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;    

    private:
        Type mType;
        sf::Sprite mSprite;
};