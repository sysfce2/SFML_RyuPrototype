#include <Ryu/Core/World.h>
#include <Ryu/Core/SpriteNode.h>
#include <Ryu/Core/Utilities.h>
#include <Ryu/Character/CharacterIchi.h>
#include <Ryu/Control/CharacterEnums.h>
#include <Ryu/Physics/DebugDraw.h>
#include <Ryu/Physics/Raycast.h>
#include <Ryu/Scene/Crate.h>

#include <Ryu/Debug/b2DrawSFML.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_draw.h>

#include <iostream>
#include <memory>
#include <array>


//namespace ryu{
World::World(sf::RenderWindow& window)
: Observer("World")
, mWindow(window)
, mWorldView(window.getDefaultView())
, mSceneTextures()
, mSceneGraph()
, mSceneLayers()
, mWorldBounds( // TODO: check what it is about bounds
    0.f,                    // left X position
    0.f,                    // top Y position
    mWorldView.getSize().x, // widthb2Color(0.9f, 0.9f, 0.4f)
    1200)//mWorldView.getSize().y)                  // height
, mSpawnPosition(
    mWorldView.getSize().x / 2.f,
    (mWorldBounds.height - mWorldView.getSize().y))
, mPushBox(nullptr)
, mPlayer(nullptr)
, phWorld(std::make_unique<b2World>(b2Vec2{0.0f,10.0f})) /// set gravity to 10 & create physics world
, phGroundBodies()
, phDebugPhysics(false)
, phTimeStep(1.f/60.f)
, clock()
{
    loadTextures();

    // build pyhsics    
    setPhysics();

    buildScene();

    mWorldView.setCenter(mSpawnPosition);

}

World::~World()
{
    mPlayer = nullptr;
    mPushBox = nullptr;
    for(const auto& body : phGroundBodies)
    {
        phWorld->DestroyBody(body);
    }
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
    mSceneTextures.load(Textures::SceneID::Grass, "assets/scenes/99_dummy/tile_grass_1.png");
    
    //mSceneTextures.load(Textures::SceneID::Ground, "assets/scenes/99_dummy/box_wood.png");
}

void
World::onNotify(const SceneNode& entity, Event event)
{
    switch(event)
    {
        case Event::DebugToggle:
        {
            toggleDrawDebug();
            break;       
        }
        default:
            break;
    }
}


// TODO: parametrize this for more level!
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

    std::unique_ptr<CharacterIchi> ichi = std::make_unique<CharacterIchi>(ECharacterState::Idle, phWorld,sf::Vector2f(100,200));
    mPlayer = ichi.get();
    mSceneLayers[static_cast<unsigned>(Layer::Foreground)]->attachChild(std::move(box));
    mSceneLayers[static_cast<unsigned>(Layer::Foreground)]->attachChild(std::move(ichi));

    //texts.emplace_back(createText("TEST"));
    setDebugDrawer(mWindow);
}

b2Body*
World::createPhysicalBox(int pos_x, int pos_y, int size_x, int size_y,
                         b2BodyType type = b2_dynamicBody, Textures::SceneID texture = Textures::SceneID::Unknown)
{
        b2BodyDef bodyDef;
        bodyDef.position.Set(Converter::pixelsToMeters<double>(pos_x)
                            ,Converter::pixelsToMeters<double>(pos_y));
        bodyDef.type = type;
        b2PolygonShape b2Shape;

        b2Shape.SetAsBox(Converter::pixelsToMeters<double>(size_x/2.0)
                        ,Converter::pixelsToMeters<double>(size_y/2.0));

        b2FixtureDef fixtureDef;                                                                                                                                                              
        fixtureDef.density = 2.0;                                                                                                                                                             
        fixtureDef.friction = 0.98;                                                                                                                                                            
        fixtureDef.restitution= 0.1;                                                                                                                                                          
        fixtureDef.shape = &b2Shape;                                                                                                                                                          
                                                                                                                                                                                              
        b2Body* res = phWorld->CreateBody(&bodyDef);                                                                                                                                             
        res->CreateFixture(&fixtureDef);                                                                                                                                                      
                                                                                                                                                                                              
        sf::Shape* shape = new sf::RectangleShape(sf::Vector2f(size_x,size_y));                                                                                                               
        shape->setOrigin(size_x/2.0,size_y/2.0);                                                                                                                                              
        shape->setPosition(sf::Vector2f(pos_x,pos_y));                                                                                                                                        
                                                                                                                                                                                              
        if(texture != Textures::SceneID::Unknown)
        {
            //shape->setFillColor(sf::Color::Red);
            //shape->setOutlineColor(sf::Color::Red);                                                                                                                                             
            //shape->setOutlineThickness(2.0f);
            shape->setTexture(&mSceneTextures.getResource(texture));
        }                                                                                                                                                            
        else                                                                                                                                                                                  
            shape->setFillColor(sf::Color::Green);                                                                                                                                            

        res->GetUserData().pointer = (uintptr_t)shape; ///OLD stalye: res->SetUserData(shape);                                                                                                                                                              
                                                                                                                                                                                              
        return res;

}

void
World::setPhysics()
{

    // grounds
    phGroundBodies.emplace_back(createPhysicalBox(600,780,1200,20,b2_staticBody));
    phGroundBodies.emplace_back(createPhysicalBox(70,150,150,32,b2_staticBody,Textures::SceneID::Grass));
    phGroundBodies.emplace_back(createPhysicalBox(240,280,140,32,b2_staticBody,Textures::SceneID::Grass));
    phGroundBodies.emplace_back(createPhysicalBox(380,380,150,32,b2_staticBody,Textures::SceneID::Grass));
    phGroundBodies.emplace_back(createPhysicalBox(500,500,120,32,b2_staticBody,Textures::SceneID::Grass));

    pBoxTest = createPhysicalBox(300,100,50,50,b2_dynamicBody,Textures::SceneID::BoxPushable);
    //sf::Shape* boxShape = getShapeFromPhysicsBody(pBoxTest);
    //newCrate.init(std::move(box),std::move(boxShape));
    //mCrates.push_back(std::move(&newCrate));

}

void
World::setDebugDrawer(sf::RenderTarget& target)
{
    // DebugDrawing 
    // Create debug drawer for window with 10x scale
    // You can set any sf::RenderTarget as drawing target
    debugDrawer.SetTarget(target);
    debugDrawer.SetScale(Converter::PIXELS_PER_METERS);

    // Set our drawer as world's drawer
    phWorld->SetDebugDraw(&debugDrawer);

    // Set flags for things that should be drawn
    // ALWAYS remember to set at least one flag,
    // otherwise nothing will be drawn
    debugDrawer.SetFlags(
        b2Draw::e_shapeBit | b2Draw::e_pairBit
    );

    
    std::cout << "SetDebugDrawer" << "\n";
    
}

sf::Shape*
World::getShapeFromPhysicsBody(b2Body* physicsBody)
{
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
    if(phDebugPhysics)
    {
        // draw raycasts
        phWorld->DebugDraw();
        for(auto rc : mPlayer->rayCastPoints)
        {
            debugDrawer.DrawSegment(rc.second.first,rc.second.second,b2Color(0.9f,0.0f,0.0f));
        }
    }

    // TODO: add the ground and stuff to the scenegraph !
    if(phGroundBodies.size() > 0)
    {
        for(const auto& body : phGroundBodies)
        {
            mWindow.draw(*(getShapeFromPhysicsBody(body)));
        }
    }
    
    if(pBoxTest)
    {
        mWindow.draw(*(getShapeFromPhysicsBody(pBoxTest)));
    }

    // Clear window
//    mWindow.clear();

    /* TODO: how to create texts ? not seem to work here
    for(const auto& text : texts)
    {
        mWindow.draw(text);   
    }

    sf::Text text;

    createText("BLUB", text);
    mWindow.draw(text);
    */   

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
World::createText(const sf::String text, sf::Text& textToShow)
{
    sf::Font font;
    if (!font.loadFromFile("assets/fonts/droid_sans.ttf"))
    {
        std::cout << "Font Error: font-file not found.\n";
    }
    

    textToShow.setString(text);

    textToShow.setCharacterSize(24); // in pixels, not points!

    textToShow.setFillColor(sf::Color::Red);

    textToShow.setPosition(sf::Vector2f{100,100});

    textToShow.setStyle(sf::Text::Bold | sf::Text::Underlined);

}

void
World::update(sf::Time dt)
{
    while(!mActiveCommands.isEmpty())
    {
        mSceneGraph.onCommand(mActiveCommands.pop(),dt);
    }
    phWorld->Step(phTimeStep,8,3);
       // Draw debug shapes of all physics objects

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