#include <SFML/Graphics/RenderWindow.hpp>

#include <Ryu/Core/World.h>
#include <Ryu/Core/SpriteNode.h>
#include <Ryu/Character/CharacterIchi.h>
#include <Ryu/Control/CharacterEnums.h>
#include <Ryu/Physics/DebugDraw.h>
#include <Ryu/Scene/Crate.h>
#include <iostream>
#include <memory>
#include <array>

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_draw.h>

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
, mPlayer(nullptr)
, phGroundBody(nullptr)
, phDebugPhysics(true)
, phTimeStep(1.f/60.f)
{
    loadTextures();

    // TODO: refine/think where to put physics
    // is this the correct place ?
    // box2D physics-world
    b2Vec2 gravity(0.0f, -10.0f);
    phWorld = std::make_unique<b2World>(gravity);
    // build pyhsics    
    setPhysics();

    buildScene();

    mWorldView.setCenter(mSpawnPosition);

}

World::~World()
{
    mPlayer = nullptr;
    mPushBox = nullptr;
    phWorld->DestroyBody(phGroundBody);
}

CharacterIchi*
World::getPlayer()
{
    return mPlayer;
}

const sf::Drawable&
World::getPlayerSprite()
{
    return mPlayer->getSpriteAnimation();
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

    std::unique_ptr<CharacterIchi> ichi = std::make_unique<CharacterIchi>(ECharacterState::Idle, phWorld,glm::vec2(1,4));
    mPlayer = ichi.get();
    mSceneLayers[static_cast<unsigned>(Layer::Foreground)]->attachChild(std::move(box));
    mSceneLayers[static_cast<unsigned>(Layer::Foreground)]->attachChild(std::move(ichi));
}

void
World::setPhysics()
{
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -2.0f); // remember this is meter!
    phGroundBody = phWorld->CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);

    phGroundBody->CreateFixture(&groundBox, 0.0f); // static object so density == 0

    uint32 flags = 0;
    flags += phDebugPhysics * b2Draw::e_shapeBit; /// draw physicShapes
    // TODO: we need to use GLFW and implement the draw methods ourself
    phWorld->SetDebugDraw(&g_debugDraw);
    g_debugDraw.SetFlags(flags);

    //Test crate, need a Texture
    Crate newCrate;
    newCrate.init(phWorld.get(),glm::vec2(0.0f,14.0f),glm::vec2(15.f,15.f));

    mCrates.push_back(&newCrate);
}

void
World::draw()
{
    mWindow.setView(mWorldView);
    // delegate work to the scenegraph
    mWindow.draw(mSceneGraph);
    // draw physics
    phWorld->DebugDraw();
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
    phWorld->Step(phTimeStep,8,3);
    //needable or already in scenegraph ?
    mPlayer->update(dt);
    mSceneGraph.update(dt);

    
    for(auto& crate : mCrates)
    {
        std::cout << crate.getBody()->GetPosition().x << crate.getBody()->GetPosition().y << "\n"; 
    }
    
   
}

//} /// namespace ryu