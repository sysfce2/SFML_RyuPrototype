#include "Ryu/Core/EventManager.h"
#include "Ryu/Physics/Physics.h"
#include "Ryu/Scene/Entity.h"
#include "Ryu/Scene/LevelManager.h"
#include "Ryu/Scene/SceneEnums.h"
#include "Ryu/Scene/SceneNode.h"
#include <Ryu/Character/CharacterIchi.h>
#include <Ryu/Control/CharacterEnums.h>
#include <Ryu/Core/SpriteNode.h>
#include <Ryu/Core/EventManager.h>
#include <Ryu/Core/Utilities.h>
#include <Ryu/Core/World.h>
//#include <Ryu/Physics/DebugDraw.h>
#include <Ryu/Physics/Raycast.h>
#include <Ryu/Scene/Crate.h>
#include <Ryu/Scene/EntityStatic.h>

#include <Ryu/Debug/b2DrawSFML.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <SFML/Graphics/Shape.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>
#include <box2d/box2d.h>
// TODO: integrate the files in the project
#include <../build_arch/_deps/tracy-src/public/tracy/Tracy.hpp>

#include <array>
#include <iostream>
#include <memory>
#include <tracy/Tracy.hpp>
#include <utility>

// namespace ryu{
World::World(sf::RenderWindow &window, EventManager& eventManager)
    : Observer("World"), mWindow(window), mWorldView(window.getDefaultView()),
      mWorldBounds(
          {0.f, 0.f},
          {mWorldView.getSize().x, 1200}),
      mSpawnPosition({mWorldView.getSize().x / 2.f,
              (mWorldBounds.size.y - mWorldView.getSize().y)}),
      mPhysics(),
      phDebugPhysics(false), clock(),
      levelManager(std::make_unique<LevelManager>()),
      mEventManager(eventManager)
 {

    mRenderer = std::make_unique<Renderer>(mWindow, mEventManager);

    // TODO: in ctor we only should do 1 thing and if the following depends on this
    // only do this when the former is finished -> or/and PRO: use multithreading
    // BUT see flow diagram blabla (WIP) SceneWorkflow: after loading textures,set physics
    // use events for this ... we have a nice observer pattern ^^
    loadTextures();

    // build pyhsics
    //OLD:
    // setPhysics();
    //NEW:
    mPhysics.createPhysicsSceneObjects(ELevel::Level2);

    // TODO: here the scene and the character is created
    // TODO: Race incoming !!!, see Game Ctor (create Ichi)
    // the scene should only be shown when everything is initalized and loaded !!!!
    // st. like: waiting on Event: PhysicsFinished, CharacterFinished, SceneFinished ...
    mRenderer->buildScene();

    mWorldView.setCenter(mSpawnPosition);

}

World::~World() {
}

const sf::Drawable &World::getPlayerSprite() {
    auto player = mEventManager.requestPlayer();
    return player->getSpriteAnimation();
}

void World::loadTextures() {
    // TODO: cant find in debug mode !
    // Textures are now loaded in Renderer
}

void World::onNotify(const SceneNode &entity, Ryu::EEvent event) {
    switch (event._value) {
    case Ryu::EEvent::DebugToggle: {
        toggleDrawDebug();
        break;
    }
    default:
        break;
    }
}

void World::setDebugDrawer(sf::RenderTarget &target) {
    // DebugDrawing
    // Create debug drawer for window with 10x scale
    // You can set any sf::RenderTarget as drawing target
    b2DrawSFML dbgDrawer;
    dbgDrawer.SetTarget(target);
    dbgDrawer.SetScale(Converter::PIXELS_PER_METERS);

    // Set flags for things that should be drawn
    // ALWAYS remember to set at least one flag,
    // otherwise nothing will be drawn
    debugDrawer.SetAllFlags();
    // Set our drawer as world's drawer
    // TODO: crashes here ?
    // mPhysics.setDebugDrawer(dbgDrawer);

}


// TODO: overthink how to store the physic body ! (in the Entityclass ?)
// --> there is now a PhysicsClass with map mScenePhysics
// whats with multiple levels .... we need at least a map
// TODO: remove when physic objects are working, not used atm
/*
b2Body *
World::createPhysicalBox(int pos_x, int pos_y, int size_x, int size_y,
                         std::string name = "EMPTY",
                         b2BodyType type = b2_dynamicBody,
                         Textures::SceneID texture = Textures::SceneID::Unknown,
                         EntityType entityType) {
    // TODO: save all in EntityMap / save adress of physicalBody in StaticEntity
    // !
    b2BodyDef bodyDef;
    bodyDef.position.Set(Converter::pixelsToMeters<double>(pos_x),
                         Converter::pixelsToMeters<double>(pos_y));
    bodyDef.type = type;
    b2PolygonShape b2Shape;

    b2Shape.SetAsBox(Converter::pixelsToMeters<double>(size_x / 2.0),
                     Converter::pixelsToMeters<double>(size_y / 2.0));

    b2FixtureDef fixtureDef;
    fixtureDef.density = 2.0;
    fixtureDef.friction = 0.98;
    fixtureDef.restitution = 0.1;
    fixtureDef.shape = &b2Shape;

    //b2Body *res = phWorld->CreateBody(&bodyDef);
    //std::unique_ptr<b2Body> res =
    // std::make_unique<b2Body>(mPhysics.createPhysicsSceneObjects(ELevel level)srt->CreateBody(&bodyDef));
    //res->CreateFixture(&fixtureDef);

    // std::unique_ptr<sf::RectangleShape> shape =
    // std::make_unique<sf::RectangleShape>(sf::Vector2f(size_x,size_y));
    // sf::RectangleShape shape{sf::Vector2f(size_x,size_y)};
    // TODO howto smartptr ?
    std::unique_ptr<sf::Shape> shape = std::make_unique<sf::RectangleShape>(sf::Vector2f(size_x, size_y));
    shape->setOrigin({(float)(size_x / 2.0), (float)(size_y / 2.0)});
    shape->setPosition(sf::Vector2f(pos_x, pos_y));

    auto staticEntity = std::make_unique<EntityStatic>(entityType);
    // std::shared_ptr<EntityStatic> staticEntity =
    // std::make_shared<EntityStatic>());
    staticEntity->setShape(std::move(shape));
    staticEntity->setName(name);

    auto shapePosition = shape->getGlobalBounds().position;
    auto shapeSize = shape->getGlobalBounds().size;
    std::vector<sf::Vector2f> cornerPoints{
        {shapePosition.x, shapePosition.y},
        {shapePosition.x + shapeSize.x, shapePosition.y},
        {shapePosition.x, shapePosition.y + shapeSize.y},
        {shapePosition.x + shapeSize.x,
         shapePosition.y + shapeSize.y}};

    staticEntity->setCornerPoints(cornerPoints);

    if (texture != Textures::SceneID::Unknown) {
        // shape->setFillColor(sf::Color::Red);
        // shape->setOutlineColor(sf::Color::Red);
        // shape->setOutlineThickness(2.0f);
        // shape->setTexture(&mSceneTextures.getResource(texture));

    } else {
        shape->setFillColor(sf::Color::Green);
    }

    // if(mStaticEntities.find(name) == mStaticEntities.end())
    //{
    // mStaticEntities.emplace(std::make_pair(name, staticEntity));
    //}
    // else
    //{
    //    fmt::print("Name {} for entity exists already.\n",name);

    //}

    // res->GetUserData().pointer = (uintptr_t)shape; ///OLD stalye:
    // res->SetUserData(shape); res->GetUserData().pointer = (uintptr_t)shape;
    // ///OLD stalye: res->SetUserData(shape);

    // res->GetUserData().pointer = reinterpret_cast<uintptr_t>(
    //    staticEntity.get()); /// OLD stalye: res->SetUserData(shape);
    //mStaticEntities[reinterpret_cast<uintptr_t>(res)] = std::move(staticEntity);

    // Dangling pointer for EntityStatic ?
    return nullptr;//res;
    //return res;
}
*/
/*
b2Body *
World::createPhysicalBox(LevelObject obj)
{
    return nullptr; //createPhysicalBox(obj.posX, obj.posY, obj.sizeX, obj.sizeY, obj.name, obj.type, obj.texture, obj.entityType);
}
*/

// TODO split in "createGroundbodies / setScenePhysics in Physics class"
void World::setPhysics() {

    // TODO: move to physicsclass !

/*
    pBoxTest =
        createPhysicalBox(750, 80, 50, 50, "box_pushable_2", b2_dynamicBody,
                          Textures::SceneID::BoxPushable);
*/
    // sf::Shape* boxShape = getShapeFromPhysicsBody(pBoxTest);
    // newCrate.init(std::move(box),std::move(boxShape));
    // mCrates.push_back(std::move(&newCrate));
}

void World::setDebugDrawer(sf::RenderTarget &target) {
    // DebugDrawing
    // Create debug drawer for window with 10x scale
    // You can set any sf::RenderTarget as drawing target
    b2DrawSFML dbgDrawer;
    dbgDrawer.SetTarget(target);
    dbgDrawer.SetScale(Converter::PIXELS_PER_METERS);

    // Set flags for things that should be drawn
    // ALWAYS remember to set at least one flag,
    // otherwise nothing will be drawn
    debugDrawer.SetAllFlags(); //SetFlags(b2Draw::e_shapeBit | b2Draw::e_pairBit);
    // Set our drawer as world's drawer
    // TODO: crashes here ?
    // mPhysics.setDebugDrawer(dbgDrawer);

}


void World::draw() {

    ZoneScopedN("Draw_World");
    mWindow.setView(mWorldView);
    // draw physics
    // TODO: phDebugPhysics && Physics::mDebugPhysicsActive ???
    if (phDebugPhysics) {
        // draw raycasts
        mPhysics.debugDraw();
        auto player = mEventManager.requestPlayer();
        player->drawRaycasts(debugDrawer); // TODO: to clarify where to move
    }

    if (phDebugPhysics) {
        mPhysics.debugDraw();
        auto player = mEventManager.requestPlayer();
        player->drawRaycasts(debugDrawer);
    }
    
    mRenderer->draw();

    if (pBoxTest) {
        // TODO: segfault
        // mWindow.draw(*(getShapeFromPhysicsBody(pBoxTest)));
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

CommandQueue &World::getActiveCommands() {
    ZoneScopedN("getActCmd_World");

    return mRenderer->getActiveCommands();
}

void World::createText(const sf::String text, sf::Text &textToShow) {
    sf::Font font;
    if (!font.openFromFile("assets/fonts/droid_sans.ttf")) {
        std::cout << "Font Error: font-file not found.\n";
    }

    textToShow.setString(text);

    textToShow.setCharacterSize(24); // in pixels, not points!

    textToShow.setFillColor(sf::Color::Red);

    textToShow.setPosition(sf::Vector2f{100, 100});

    textToShow.setStyle(sf::Text::Bold | sf::Text::Underlined);
}

void World::update(sf::Time dt) {

	ZoneScopedN("Update_World");

    mPhysics.update();
    // Draw debug shapes of all physics objects

    // needable or already in scenegraph ?
    // TODO: is this efficient ?
    // we basically use a sharedptr of plyer in update, question is if we need this here
    // maybe here is also the discrepance btw. physics and characterassetmovement ! please investigate    mEventManager.requestPlayer()->update(dt); //slowmo
    mRenderer->update(dt);

    /*
    for(auto& crate : mCrates)
    {
        std::cout << crate.getBody()->GetPosition().x <<
    crate.getBody()->GetPosition().y << "\n";
    }
    */
}

//} /// namespace ryu
