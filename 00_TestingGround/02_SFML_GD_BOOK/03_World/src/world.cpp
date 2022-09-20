#include "world.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>
#include <array>
#include "spritenode.h"

World::World(sf::RenderWindow& window)
: mWindow(window)
, mWorldView(window.getDefaultView())
, mWorldBounds(
    0.f,                    // left X position
    0.f,                    // top Y position
    mWorldView.getSize().x, // width
    200.f)                  // height
, mSpawnPosition(
    mWorldView.getSize().x / 2.f,
    (mWorldBounds.height - mWorldView.getSize().y))
, mPlayerAircraft(nullptr)
{
    loadTextures();
    buildScene();

    mWorldView.setCenter(mSpawnPosition);
}

void
World::loadTextures()
{
    mTextures.load(Textures::ID::Eagle, "Media/Textures/Eagle.png");
    mTextures.load(Textures::ID::Raptor, "Media/Textures/Raptor.png");
    mTextures.load(Textures::ID::Desert, "Media/Textures/Desert.png");
}

void
World::buildScene()
{
    // set Layer
    for(std::size_t i = 0; i < size_t(Layer::LayerCount); ++i)
    {
        SceneNode::Ptr layer(new SceneNode());
        mSceneLayers[i] = layer.get();

        mSceneGraph.attachChild(std::move(layer));
    }

    sf::Texture& texture = mTextures.getResource(Textures::ID::Desert);
    sf::IntRect textureRect(mWorldBounds);
    //texture.setRepeated(true);

    std::unique_ptr<SpriteNode> backgroundSprite = std::make_unique<SpriteNode>(texture, textureRect);
    backgroundSprite->setPosition(
        mWorldBounds.left,
        mWorldBounds.top
    );
    mSceneLayers[std::size_t(Layer::Background)]->attachChild(std::move(backgroundSprite));

    std::unique_ptr<Aircraft> leader = std::make_unique<Aircraft>(Aircraft::Type::Eagle, mTextures);
    mPlayerAircraft = leader.get();
    mPlayerAircraft->setPosition(mSpawnPosition);
    mPlayerAircraft->setVelocity(40.f, mScrollSpeed);
    mSceneLayers[std::size_t(Layer::Air)]->attachChild(std::move(leader));

    std::unique_ptr<Aircraft> leftEscort = std::make_unique<Aircraft>(Aircraft::Type::Raptor, mTextures);
    leftEscort->setPosition(-80.f,50.f);
    mPlayerAircraft->attachChild(std::move(leftEscort));

    std::unique_ptr<Aircraft> rightEscort = std::make_unique<Aircraft>(Aircraft::Type::Raptor, mTextures);
    rightEscort->setPosition(80.f,50.f);
    mPlayerAircraft->attachChild(std::move(rightEscort));
}

void
World::draw()
{
    mWindow.setView(mWorldView);
    // delegate work to the scenegraph
    mWindow.draw(mSceneGraph);
}

void
World::update(sf::Time dt)
{
    mWorldView.move(0.f, mScrollSpeed * dt.asSeconds());

    sf::Vector2f position = mPlayerAircraft->getPosition();
    sf::Vector2f velocity = mPlayerAircraft->getVelocity();

    if ((position.x <= mWorldBounds.left + 150)
    || (position.x >= mWorldBounds.left + mWorldBounds.width - 150))
    {
        velocity.x = -velocity.x;
        mPlayerAircraft->setVelocity(velocity);
    }

    mSceneGraph.update(dt);   


}