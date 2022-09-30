#include <Ryu/Core/World.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>
#include <array>
#include <Ryu/Core/SpriteNode.h>
#include <iostream>

//namespace ryu{

World::World(sf::RenderWindow& window)
: mWindow(window)
, mWorldView(window.getDefaultView())
, mSceneTextures()
, mSceneGraph()
, mSceneLayers()
, mWorldBounds( // TODO: check what it is about bounds
    0.f,                    // left X position
    0.f,                    // top Y position
    mWorldView.getSize().x, // width
    1200)//mWorldView.getSize().y)                  // height
, mSpawnPosition(
    mWorldView.getSize().x / 2.f,
    (mWorldBounds.height - mWorldView.getSize().y))
, mPushBox(nullptr)
{
    loadTextures();
    buildScene();

    mWorldView.setCenter(mSpawnPosition);
}

void
World::loadTextures()
{
    // TODO: cant find in debug mode !
    mSceneTextures.load(Textures::SceneID::BoxPushable, "assets/scenes/99_dummy/box_wood.png");
    mSceneTextures.load(Textures::SceneID::BGMountain, "assets/backgrounds/99_dummy/722756.png");
    
    //mSceneTextures.load(Textures::SceneID::Ground, "assets/scenes/99_dummy/box_wood.png");
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

    sf::Texture& textureBg = mSceneTextures.getResource(Textures::SceneID::BGMountain);
    sf::IntRect textureRect(mWorldBounds);
    //textureBg.setRepeated(true);

    std::unique_ptr<SpriteNode> backgroundSprite = std::make_unique<SpriteNode>(textureBg, textureRect);
    backgroundSprite->setPosition(
        mWorldBounds.left,
        mWorldBounds.top
    );
    mSceneLayers[static_cast<unsigned>(Layer::Ground1)]->attachChild(std::move(backgroundSprite));

    // pushable Box
    std::unique_ptr<Box> box = std::make_unique<Box>(Box::Type::Pushable, mSceneTextures);
    mPushBox = box.get();
    mPushBox->setPosition(mSpawnPosition);
    mSceneLayers[static_cast<unsigned>(Layer::Foreground)]->attachChild(std::move(box));
    
}

void
World::draw()
{
    mWindow.setView(mWorldView);
    // delegate work to the scenegraph
    mWindow.draw(mSceneGraph);
}

CommandQueue&
World::getActiveCommands()
{
    return mActiveCommands;
}

void
World::update(sf::Time dt)
{
    while(!mActiveCommands.isEmpty())
    {
        mSceneGraph.onCommand(mActiveCommands.pop(),dt);
    }

    mSceneGraph.update(dt);   
}

//} /// namespace ryu