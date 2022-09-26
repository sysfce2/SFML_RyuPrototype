#include "aircarft.h"
#include "category.h"

Textures::ID toTextureID(Aircraft::Type type)
{
    switch (type)
    {
        case Aircraft::Type::Eagle:
            return Textures::ID::Eagle;
        case Aircraft::Type::Raptor:
            return Textures::ID::Raptor;
    }
}

Aircraft::Aircraft(Type type, const TextureHolder& textures) 
    : mType(type), mSprite(textures.getResource(toTextureID(type)))
{
    // set origin of texture to center
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void 
Aircraft::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}

void
Aircraft::accelerate(sf::Vector2f velocity)
{
    setVelocity(getVelocity()+ velocity);
}

unsigned int 
Aircraft::getCategory() const
{
    switch (mType)
    {
        case Type::Eagle:
        {
            return static_cast<unsigned>(Category::Type::Player);
        }
        case Type::Raptor:
        {
            return static_cast<unsigned>(Category::Type::Ally);
        }
       
    default:
            return static_cast<unsigned>(Category::Type::Enemy);
   } 
} 