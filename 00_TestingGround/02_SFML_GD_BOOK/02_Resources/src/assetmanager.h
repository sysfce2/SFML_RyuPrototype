#pragma once

#include <map>
#include <memory>
#include <SFML/Graphics.hpp>

namespace Textures
{
    enum class ID {
        Landscape,
        Airplane,
        Missile
    };
}

class TextureHolder
{
    public:
        void load(Textures::ID id, const std::string& filename);
        sf::Texture& getTexture(Textures::ID id);
        const sf::Texture& getTexture(Textures::ID id) const;

    private:
        std::map<Textures::ID, std::unique_ptr<sf::Texture> > mTextureMap;
};