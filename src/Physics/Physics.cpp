#include "Ryu/Core/AssetIdentifiers.h"
#include "Ryu/Debug/b2DrawSFML.hpp"
#include "Ryu/Scene/EntityStatic.h"
#include "Ryu/Scene/SceneEnums.h"
#include <Ryu/Physics/Physics.h>

#include <Ryu/Core/Utilities.h>

#include <Ryu/Character/ICharacter.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/System/Vector2.hpp>
#include <box2d/box2d.h>
#include <box2d/types.h>
#include <exception>
#include <fmt/core.h>
#include <memory>


const b2Vec2 GRAVITY(0,9.81f);
constexpr float PHYSICS_TIME_STEP = 1.f / 60.f;
constexpr int SUBSTEP_COUNT = 1;
constexpr int32_t VELOCITY_ITERATIONS = 8;
constexpr int32_t POSITION_ITERATIONS = 3;

CharacterPhysicsParameters::CharacterPhysicsParameters() :
    mBody(nullptr),
    mFixture(nullptr),
    mGravityScale(4.8f),  /// for dynamic objects density needs to be > 0
    mDensity(5.f), /// for dynamic objects density needs to be > 0
    mFriction(0.1f), /// recommended by  b2d docu
    mFixtureRestitution(0.1f),
    mRaycastLength(40.0f),
    mMoveMultiplier({1.05f, 1.47f}),
    mJumpForwardImpulse({150, -250}),
    mJumpUpImpulse({0, -200}),
    mMassCenter({0, 0}),
    mBodyMass(25)
{}

CharacterPhysicsParameters::~CharacterPhysicsParameters()
{
    b2DestroyBody(mBodyId);
    mBodyId = b2_nullBodyId;
}

SceneObjectPhysicsParameters::SceneObjectPhysicsParameters() :
    mPosition({}),
    mSize({}),
    mName(""),
    mType(b2BodyType::b2_staticBody),
    mTextureId(Textures::SceneID::Unknown),
    mEntityType(EntityType::None),
    mPhysicsBodyId(b2_nullBodyId) {}
    // polygonShape.SetAsBox(0.5,0.9);


SceneObjectPhysicsParameters::SceneObjectPhysicsParameters(
        sf::Vector2i position, sf::Vector2i size,
        std::string name, b2BodyType type,
        Textures::SceneID textureId, EntityType entityType) :
    mPosition(position),
    mSize(size),
    mName(name),
    mType(type),
    mTextureId(textureId),
    mEntityType(entityType),
    mPhysicsBodyId(b2_nullBodyId)
{}

// TODO: set contactlistener (see characterbase)
Physics::Physics() :
    mCharacterPhysics({}),
    mPhysicsWorldId(),
    mStaticEntities(),
    mPhTimeStep(PHYSICS_TIME_STEP),
    mDebugPhysicsActive(false)
{
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = GRAVITY;
    mPhysicsWorldId = b2CreateWorld(&worldDef);
    
}

Physics::~Physics()
{
   for(auto& level : sceneObjects)
   {
       for(auto& sceneObj : level.second)
       {
           // TODO: save every bodyId in a map
           // b2DestroyBody(sceneObj.mBodyId);
           // mPhysicsWorld->DestroyBody(sceneObj.mPhysicsBody);
       }
   }

   b2DestroyWorld(mPhysicsWorldId);
   mPhysicsWorldId = b2_nullWorldId;
}

void
Physics::update()
{
    b2World_Step(mPhysicsWorldId, PHYSICS_TIME_STEP, SUBSTEP_COUNT); //VELOCITY_ITERATIONS, POSITION_ITERATIONS);
}

void
Physics::setDebugPhysics(bool debugPhysics)
{
    mDebugPhysicsActive = debugPhysics;
}

void
Physics::debugDrawSegment(b2Vec2 const &p1, b2Vec2 const &p2,
                          b2HexColor const &color) const
{
    if (mDebugPhysicsActive)
    {
        debugDrawer.DrawSegment(p1, p2, color);
    }
}

void
Physics::debugDraw() const
{
    if(mDebugPhysicsActive)
    {
        // TODO: how to draw physics ?
        // mPhysicsWorld->DebugDraw();
    }
}

void
Physics::draw(sf::RenderWindow& window)
{

    // TODO: mPhysicsBody is nullptr but is set in CreatePhysicsBody!
    // thats why thy bodies cant be displayed
    // so probably not set correctly or destroyed when going out of scope
    // before the refactoring the physicsbodies were emplaced in an physicsbodyarray
    // and created there directly
    // TODO: next step ask with help on ChatGTP what could be wrong in this situation
    if (sceneObjects.size() > 0) {
        for (auto& obj : sceneObjects.at(ELevel::Level2)) {
            fmt::print("draw: get PhysicsBody: {} \n",b2Body_IsValid(obj.mPhysicsBodyId) ? "no there" : "is there");
            auto shape = getShapeFromPhysicsBody(obj.mPhysicsBodyId);
            if (shape == nullptr) {
                fmt::print("shape ptr seems to be null\n");
                fmt::print("damn\n");
                return;
            }

            window.draw(*(shape));
        }
    }
}

sf::Shape*
Physics::getShapeFromPhysicsBody(b2BodyId physicsBodyId) {
    if (b2Body_IsValid(physicsBodyId))
        return nullptr;

    auto user_data = b2Body_GetUserData(physicsBodyId);
    auto entity = reinterpret_cast<EntityStatic*>(user_data);
    /*
    auto body = reinterpret_cast<uintptr_t>(physicsBody);
    sf::Shape* shape =
    reinterpret_cast<sf::RectangleShape*>(mStaticEntities.at(body)->getShape());
    */

    sf::Shape* shape =
        reinterpret_cast<sf::RectangleShape*>(entity->getShape());

    if (shape) {

        try {
            auto bodyPos = b2Body_GetPosition(physicsBodyId);
            auto bodyRot = b2Body_GetRotation(physicsBodyId);
            float angleInRadians = b2Rot_GetAngle(bodyRot);
            shape->setPosition({
                Converter::metersToPixels(bodyPos.x),
                Converter::metersToPixels(bodyPos.y)});
            shape->setRotation(
                sf::degrees(Converter::radToDeg<double>(angleInRadians)));
        } catch (std::exception) {
            fmt::print("No shape.\n");
        }
    } else {
        fmt::print("shape null.\n");
        return nullptr;
    }
    return shape;
}

void
Physics::initCharacterPhysics(ICharacter& character, bool inDuckMode)
 {
// TODO: CAUTION:: THERE is an thinking error and a cyclic dependency inside, TOO LATE !!!
   // TODO:
   // - initialize needed values in iniatlizer list of ctor
   // - get implementation from CharacterBase::initPhysics(...)
   // - call accordingly
   //-    // TODO: make it adjustable ? or remove and add new ? -> e.g. duck state ->
    // halfPhysics box
    // init physics after the charactersprite was created !
    // Create the body of the falling Crate
    auto position = character.getPosition();

    const auto shapeSize = inDuckMode ? DUCK_FRAME_SIZE : INIT_FRAME_SIZE;
    int size_x
        = shapeSize
              .first; // mCharacterAnimation.getSprite().getTextureRect().width;
    int size_y = shapeSize.second;

    CharacterPhysicsParameters& charPhysics = mCharacterPhysics.at(character.getCharacterName());

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody; /// TODO: or even kinematic body ?
    bodyDef.position.x = Converter::pixelsToMeters<double>(position.x);
    bodyDef.position.y = Converter::pixelsToMeters<double>(
                             inDuckMode
                                 ? position.y + (DUCK_FRAME_SIZE.second / 2)
                                 : position.y);
    bodyDef.fixedRotation = true;
    bodyDef.gravityScale = charPhysics.mGravityScale;

    charPhysics.mBodyId = b2CreateBody(mPhysicsWorldId, &bodyDef);
    
    // Create a the shape
    b2Polygon polygonBoxShape = b2MakeBox(Converter::pixelsToMeters<double>(size_x * 0.5f), Converter::pixelsToMeters<double>(size_y * 0.5f));
    b2ShapeDef shapeDef = b2DefaultShapeDef(); // initialize shapedefinition
    shapeDef.density = charPhysics.mDensity;
    //shapeDef.friction = charPhysics.mFriction; deprecated since Box2D 3.1, different concept


    //shapeDef.userData = (uintptr_t)shape;

    charPhysics.mShapeDef = shapeDef;
    charPhysics.mShapeId = b2CreatePolygonShape(charPhysics.mBodyId, &shapeDef, &polygonBoxShape);
    //charPhysic.mShapeId. = (uintptr_t)shape; //.get();
    
    // SFML shape 
    // TODO: with SFML 3 there are probably smartPointer more elegant ? otherwise do RAII !
    sf::Shape *shape = new sf::RectangleShape(sf::Vector2f(size_x, size_y));
    // or can we delete this/free this at the end of the function ? bc its casted to uintptr_t
    // TODO: test it
    // std::unique_ptr<sf::Shape> shape =
    // std::make_unique<sf::RectangleShape>(sf::Vector2f(size_x, size_y));
    shape->setOrigin({size_x / 2.0f, size_y / 2.0f});
    shape->setPosition(sf::Vector2f(position.x, position.y));
    // TODO check if we need setting a texture
    // shape->setTexture(
    //    &baseTextureManager.getResource(Textures::PhysicAssetsID::Empty));

    b2Shape_SetUserData(charPhysics.mShapeId, &shape);
    //b2ShapeId shapeId = b2Body_AddShape(charPhysics.mBodyId, )
    
    fmt::print("Init character at position {},{}\n",
               Converter::metersToPixels(bodyDef.position.x),
               Converter::metersToPixels(bodyDef.position.y));
    // mBody->SetLinearVelocity(b2Vec2(0.0f, -50.0f));

    // insert physics according to character
    mCharacterPhysics.insert(
        std::make_pair(character.getCharacterName(), charPhysics)
    );
}

void
Physics::createPhysicsSceneObjects(ELevel level)
{
    int i = 0;
    for(auto obj : sceneObjects.at(level))
    {
        fmt::print("create: {} \n", obj.mName);
        auto physObj = createPhysicsBody(obj, i);
        obj.mPhysicsBodyId = physObj;
        // phGroundBodies.emplace_back(PhysicsObject("", createPhysicalBox(obj)));
    }
}

// TODO: make method acc Box2D v3.1 !
// and also clean / make it easier !, see InitCharacterPhysics, check what can be combined
// lambda or helpermethods
b2BodyId
Physics::createPhysicsBody(SceneObjectPhysicsParameters& sceneObject, int& i)
//Physics::createPhysicsBody(SceneObjectPhysicsParameters sceneObject)
{
    sf::Vector2i objPosition = sceneObject.mPosition;
    sf::Vector2i objSize = sceneObject.mSize;

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.position = (b2Vec2){Converter::pixelsToMeters<double>(objPosition.x), Converter::pixelsToMeters<double>(objPosition.y)};
    bodyDef.type = sceneObject.mType;
    
    if(B2_IS_NULL(mPhysicsWorldId)){
        fmt::print("no physics world created\n");
        return b2_nullBodyId;
    }

    b2BodyId bodyId = b2CreateBody(mPhysicsWorldId, &bodyDef);

    b2ShapeDef shapeDef = b2DefaultShapeDef(); // initialize shapedefinition
    shapeDef.density = 2.0; // TODO make it configurable ?
    // shapeDef.friction = 0.98; // deprecated since Box2D 3.1
    // fixtureDef.restitution = 0.1;
    
    b2Polygon box = b2MakeBox(Converter::pixelsToMeters<double>(objSize.x / 2.0), Converter::pixelsToMeters<double>(objSize.y / 2.0));

    b2ShapeId shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &box);
    // std::unique_ptr<sf::RectangleShape> shape =
    // std::make_unique<sf::RectangleShape>(sf::Vector2f(size_x,size_y));
    // sf::RectangleShape shape{sf::Vector2f(size_x,size_y)};
    // TODO howto smartptr ?
    // sf::Shape *shape = new sf::RectangleShape(objSize);
    std::unique_ptr<sf::Shape> shape =
        std::make_unique<sf::RectangleShape>(sf::Vector2f(objSize));

    shape->setOrigin({(float)(objSize.x / 2.0), (float)(objSize.y / 2.0)});
    shape->setPosition(sf::Vector2f(objPosition.x, objPosition.y));

    if (sceneObject.mTextureId != Textures::SceneID::Unknown) {
         shape->setFillColor(sf::Color::Red);
         shape->setOutlineColor(sf::Color::Red);
         shape->setOutlineThickness(2.0f);
        // TODO: how to set texture without coupling to texturestuff
        // this can be probably be part of the Levelmanager and to LM we need a link
        // from World and from Physics ... ? MAYBE
        // shape->setTexture(&mSceneTextures.getResource(texture));
    }
    else {
        shape->setFillColor(sf::Color::Green);
    }

    b2Shape_SetUserData(shapeId, &shape);
    
    // TODO: check what is staticEntity for
    auto staticEntity = std::make_unique<EntityStatic>(sceneObject.mEntityType);
    // std::shared_ptr<EntityStatic> staticEntity =
    // std::make_shared<EntityStatic>());
    staticEntity->setName(sceneObject.mName);

    auto shapePosition = shape->getGlobalBounds().position;
    auto shapeSize = shape->getGlobalBounds().size;
    std::vector<sf::Vector2f> cornerPoints{
        {shapePosition.x, shapePosition.y},
        {shapePosition.x + shapeSize.x, shapePosition.y},
        {shapePosition.x, shapePosition.y + shapeSize.y},
        {shapePosition.x + shapeSize.x,
         shapePosition.y + shapeSize.y}};

    staticEntity->setCornerPoints(cornerPoints);
    // TODO: is this up to date and needable ?
    staticEntity->setShape(std::move(shape));
    // res->GetUserData().pointer = reinterpret_cast<uintptr_t>(staticEntity.get());
    mStaticEntities[bodyId] = std::move(staticEntity);
    //mStaticEntities[reinterpret_cast<uintptr_t>(res)] = staticEntity;

    // Dangling pointer for EntityStatic ?
    //sceneObject.mPhysicsBody = res;
    // TODO: find out why the physbody is not copied to sceneObjects ! sie print debug, its in obj but not in the map
    // investigate further with state from before (see MR)
    // is it maybe bc of the definition of the objects (many values are predefined in SceneObjectPhysicsParameters::SceneObjectPhysicsParameters() :
    // tryo so set them by hand at creation in the map ....
    fmt::print("Before set: Physicsbody for {} set ,{}, i={} \n",sceneObject.mName, B2_IS_NULL(sceneObject.mPhysicsBodyId) ? "null_BodyId" : "physBody exists", i);
    fmt::print("{} sceneObjects.at({}): , {} \n",i,sceneObjects.at(ELevel::Level2)[i].mName, B2_IS_NULL(sceneObjects.at(ELevel::Level2)[i].mPhysicsBodyId) ? "physbody==null_BodyId" : "physBody exists");
    //fmt::print("BodyID: {} \n", bodyId);
    sceneObject.mPhysicsBodyId = bodyId;
    fmt::print("After set: Physicsbody for {} set ,{} \n",sceneObject.mName, B2_IS_NULL(sceneObject.mPhysicsBodyId) ? "null_BodyId" : "physBody exists");
    i++;
    return bodyId;

}

void
Physics::setDebugDrawer(b2DrawSFML dbgDrawer)
{
    /* TODO: How to set the Tags in Box2d v3.1 ?
    debugDrawer.SetTarget(dbgDrawer.GetRenderTarget());
    debugDrawer.SetScale(dbgDrawer.GetScale());
    debugDrawer.SetFlags(dbgDrawer.GetFlags());
    mPhysicsWorld->SetDebugDraw(&debugDrawer);// was located as static in World
    */
}
