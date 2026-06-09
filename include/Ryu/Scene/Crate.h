#pragma once

#include <Ryu/Core/AssetManager.h>
#include <Ryu/Core/AssetIdentifiers.h>

// TODO: lighten the header vor Box2D / glm a bit ?
#include <Thirdparty/glm/glm.hpp>
#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>


typedef AssetManager<sf::Texture, Textures::PhysicAssetsID> CrateTextureManager;

class Crate : public sf::Sprite
{
public:
    Crate();
    ~Crate();

    void init(b2WorldId phWorld, const glm::vec2 &position, const glm::vec2 &dimension);
    void init(b2BodyId pBody, sf::Shape* pShape);

    b2BodyId getBody(){return mBodyId;}
    sf::Shape* getShape(){return mShape;}

private:
    b2BodyId mBodyId;
    sf::Texture* mTexture;
    sf::Shape* mShape;

    CrateTextureManager mCrateTextureManager;
};
