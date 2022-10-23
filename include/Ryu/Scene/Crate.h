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

    void init(b2World* phWorld, const glm::vec2 &position, const glm::vec2 &dimension);

    b2Body* getBody(){return mBody;}
    b2Fixture* getFixture(){return mFixture;}

private:
    b2Body* mBody;
    b2Fixture* mFixture;
    sf::Texture* mTexture;

    CrateTextureManager mCrateTextureManager;
};