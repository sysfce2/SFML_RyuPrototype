#include "assetmanager.h"
#include <memory>
#include <map>

sf::Texture&
TextureHolder::getTexture(Textures::ID id)
{
    auto textureFound = mTextureMap.find(id);
    return *textureFound->second;
}


const sf::Texture& 
TextureHolder::getTexture(Textures::ID id) const
{
    return getTexture(id);
}



void 
TextureHolder::load(Textures::ID id, const std::string& filename)
{
    auto texture = std::make_unique<sf::Texture>();
    texture->loadFromFile(filename);

    // gove ownership to TextureMap
    mTextureMap.insert(std::make_pair(id,std::move(texture)));
}