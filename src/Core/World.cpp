#include <Ryu/Core/World.h>
#include <Ryu/Core/SpriteNode.h>
#include <Ryu/Core/Utilities.h>
#include <Ryu/Character/CharacterIchi.h>
#include <Ryu/Control/CharacterEnums.h>
#include <Ryu/Physics/DebugDraw.h>
#include <Ryu/Scene/Crate.h>

#include <SFML/Graphics/RenderWindow.hpp>

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_draw.h>

#include <iostream>
#include <memory>
#include <array>

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
    b2Vec2 gravity(0.0f, 10.0f);
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

b2Body*
World::createPhysicalBox(int pos_x, int pos_y, int size_x, int size_y, b2BodyType type = b2_dynamicBody)
{
        b2BodyDef bodyDef;
        bodyDef.position.Set(Converter::pixelsToMeters<double>(pos_x)
                            ,Converter::pixelsToMeters<double>(pos_y));
        bodyDef.type = type;
        b2PolygonShape b2Shape;

        b2Shape.SetAsBox(Converter::pixelsToMeters<double>(size_x/2.0)
                        ,Converter::pixelsToMeters<double>(size_y/2.0));

        b2FixtureDef fixtureDef;                                                                                                                                                              
        fixtureDef.density = 1.0;                                                                                                                                                             
        fixtureDef.friction = 0.1;                                                                                                                                                            
        fixtureDef.restitution= 0.2;                                                                                                                                                          
        fixtureDef.shape = &b2Shape;                                                                                                                                                          
                                                                                                                                                                                              
        b2Body* res = phWorld->CreateBody(&bodyDef);                                                                                                                                             
        res->CreateFixture(&fixtureDef);                                                                                                                                                      
                                                                                                                                                                                              
        sf::Shape* shape = new sf::RectangleShape(sf::Vector2f(size_x,size_y));                                                                                                               
        shape->setOrigin(size_x/2.0,size_y/2.0);                                                                                                                                              
        shape->setPosition(sf::Vector2f(pos_x,pos_y));                                                                                                                                        
                                                                                                                                                                                              
        if(type == b2_dynamicBody)
        {
            //shape->setFillColor(sf::Color::Red);
            //shape->setOutlineColor(sf::Color::Red);                                                                                                                                             
            //shape->setOutlineThickness(2.0f);
            shape->setTexture(&mSceneTextures.getResource(Textures::SceneID::BoxPushable));
        }                                                                                                                                                            
        else                                                                                                                                                                                  
            shape->setFillColor(sf::Color::Green);                                                                                                                                            

        res->GetUserData().pointer = (uintptr_t)shape; ///OLD stalye: res->SetUserData(shape);                                                                                                                                                              
                                                                                                                                                                                              
        return res;

}

void
World::setPhysics()
{
    phGroundBody = createPhysicalBox(600,780,1000,20,b2_staticBody);
    
    
    //Test crate, need a Texture
    //Crate newCrate;
    //newCrate.init(phWorld.get(),glm::vec2(0.0f,14.0f),glm::vec2(15.f,15.f));
    pBoxTest = createPhysicalBox(300,100,64,64);
    //sf::Shape* boxShape = getShapeFromPhysicsBody(pBoxTest);
    //newCrate.init(std::move(box),std::move(boxShape));

    //mCrates.push_back(std::move(&newCrate));
    
}

sf::Shape*
World::getShapeFromPhysicsBody(b2Body* physicsBody)
{
// TODO: extract own method : draw physics ground
    b2BodyUserData& data = physicsBody->GetUserData();
    sf::Shape* shape = reinterpret_cast<sf::RectangleShape*>(data.pointer);

    shape->setPosition(Converter::metersToPixels(physicsBody->GetPosition().x),
                       Converter::metersToPixels(physicsBody->GetPosition().y));
    shape->setRotation(Converter::radToDeg<double>(physicsBody->GetAngle()));
    return shape;
}

void
World::draw()
{
    mWindow.setView(mWorldView);
    // delegate work to the scenegraph
    mWindow.draw(mSceneGraph);
    // draw physics
    //phWorld->DebugDraw();

    // TODO: add the ground and stuff to the scenegraph !
    mWindow.draw(*(getShapeFromPhysicsBody(phGroundBody)));
    
    mWindow.draw(*(getShapeFromPhysicsBody(pBoxTest)));
    
    /*
    for(const auto& crate : mCrates)
    {
        mWindow.draw(*(getShapeFromPhysicsBody(crate->getBody())));
    }
    */
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

    /*
    for(auto& crate : mCrates)
    {
        std::cout << crate.getBody()->GetPosition().x << crate.getBody()->GetPosition().y << "\n"; 
    }
    */
   
}

//} /// namespace ryu