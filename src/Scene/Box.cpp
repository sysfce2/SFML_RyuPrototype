#include <Ryu/Scene/Box.h>
#include <Ryu/Core/AssetIdentifiers.h>
#include <Ryu/Core/AssetManager.h>

namespace ryu {

Textures::SceneID toTextureID(Box::Type type)
{
    switch (type)
    {
        case Box::Type::Pushable:
            return Textures::SceneID::BoxPushable;
        case Box::Type::Heavy:
            return Textures::SceneID::BoxHeavy;
        default:
            return Textures::SceneID::Unknown;
    }
}

Box::Box(Type boxType, const SceneTextureHolder& textures) 
    : mType(boxType), mSprite(textures.getResource(toTextureID(boxType)))
{
    // TODO: add another ctor for positioning !
    
    // set origin of texture to center 
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void 
Box::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}

} /// namespace ryu