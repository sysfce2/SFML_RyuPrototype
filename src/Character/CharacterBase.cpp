#include "Ryu/Animation/JsonParser.h"
#include "Ryu/Animation/SpritesheetAnimation.h"
#include "Ryu/Control/CharacterEnums.h"
#include "Ryu/Core/AssetIdentifiers.h"
#include "Ryu/Statemachine/CharacterStateLedgeClimbUp.h"
#include "Ryu/Statemachine/CharacterStateLedgeHang.h"
#include <Ryu/Animation/AnimationManager.h>
#include <Ryu/Character/CharacterBase.h>
#include <Ryu/Core/AssetManager.h>
#include <Ryu/Core/Utilities.h>
#include <Ryu/Statemachine/CharacterStateFalling.h>
#include <Ryu/Statemachine/CharacterStateIdle.h>

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <Thirdparty/glm/glm.hpp>
#include <box2d/b2_math.h>
#include <box2d/b2_shape.h>
#include <fmt/core.h>

#include <box2d/box2d.h>

#include <algorithm>
#include <iostream>
#include <memory>

// namespace ryu {

// this is the framesize for the boundary box of the physics body
static constexpr std::pair<int, int> INIT_FRAME_SIZE(60, 86);
static constexpr std::pair<int, int> DUCK_FRAME_SIZE(60, 45);

template <typename T>
bool IsInBounds(const T &value, const T &low, const T &high) {
    return !(value < low) && !(high < value);
}

CharacterBase::CharacterBase(std::unique_ptr<b2World> &phWorld,
                             const sf::Vector2f &position)
    : Observer("CharacterBase"),
      mCharacterState(std::make_unique<CharacterStateIdle>()),
      movement(0.f, 0.f), mMoveDirection(EMoveDirection::Right),
      phWorldRef(phWorld), mCharacterFalling(false), baseTextureManager(),
      mCharSettings(), mCurrentLevel(Textures::LevelID::Level1),
      rayCastPoints(), mCharacterAnimation(this),
      mECharacterState(ECharacterState::None),
      mDuckStateActive(false),
      mAnimationManager(std::make_unique<AnimationManager>()),
      mSetOffset(false),
      contactListener()
{
    // std::shared_ptr<CharacterBase> sPtr =
    // std::make_shared<CharacterBase>(this); mCharacterAnimation =
    // SpritesheetAnimation(std::move(sPtr));
    loadTextures();
    phWorldRef->SetContactListener(&contactListener);
}

CharacterBase::CharacterBase(ECharacterState startState,
                             std::unique_ptr<b2World> &phWorld,
                             const sf::Vector2f &position)
    : Observer("CharacterBase"), mECharacterState(startState),
      mCharacterState(std::make_unique<CharacterStateIdle>()),
      mCharacterSpeed(55.0f) // startvalue playerspeed
      ,
      mMoveDirection(EMoveDirection::Right), phWorldRef(phWorld),
      mCharacterFalling(false), baseTextureManager(), mCharSettings(),
      mCurrentLevel(Textures::LevelID::Level1),mDuckStateActive(false), mCharacterAnimation(this),
      mAnimationManager(std::make_unique<AnimationManager>()),
      mSetOffset(false),
      contactListener()
{
    // std::shared_ptr<CharacterBase> sPtr =
    // std::make_shared<CharacterBase>(this); mCharacterAnimation =
    // SpritesheetAnimation(std::move(sPtr));
    // TODO: check if its needable&possible to start character from a certain
    // state
    loadTextures();
    phWorldRef->SetContactListener(&contactListener);
}

void CharacterBase::setCharacterSettings(CharacterSetting settings) {
	fmt::print("useDebugValues: JF: {}/{}, JU: {}/{}\n",
               settings.jumpForwardImpulse.x, settings.jumpForwardImpulse.y,
               settings.jumpUpImpulse.x, settings.jumpUpImpulse.y);

    mCharSettings.jumpForwardImpulse = settings.jumpForwardImpulse;
    mCharSettings.jumpUpImpulse = settings.jumpUpImpulse; // TODO: probably we need to convert st here;
}


void CharacterBase:: setPositionOffset(sf::Vector2f offset) {
/*
    if (positionCrossOffset.x == 0 && positionCrossOffset.y == 0) positionCrossOffset = mCharacterAnimation.getPosition();

    mCharacterAnimation.setPosition({positionCrossOffset.x+offset.x,positionCrossOffset.y+offset.y});
*/
    lastPositionCrossOffset = positionCrossOffset;
    positionCrossOffset.x = offset.x;
    positionCrossOffset.y = offset.y;
    fmt::print("PosiCross Char: {}/{} \n Offset: {}/{}\n",positionCrossOffset.x, positionCrossOffset.y, offset.x, offset.y);
}

void
CharacterBase::resetCharacterSettings()
{
    fmt::print("reset Values");
    mCharSettings.MoveMultiplierX = mFinalCharSettings.MoveMultiplierX;
    mCharSettings.MoveMultiplierY = mFinalCharSettings.MoveMultiplierY;
    mCharSettings.jumpUpImpulse = mFinalCharSettings.jumpUpImpulse;
    mCharSettings.jumpForwardImpulse = mFinalCharSettings.jumpForwardImpulse;
    mCharSettings.bodyMass = mFinalCharSettings.bodyMass;
    mCharSettings.massCenter = mFinalCharSettings.massCenter;

    positionCrossOffset.x = 0.f;
    positionCrossOffset.y = 0.f;
}

void CharacterBase::updatePhysics() {
    sf::Vector2f position = mCharacterAnimation.getPosition();

    // initPhysics(phWorldRef,position);
}

void CharacterBase::updatePhysics(const sf::Vector2f &position) {
    // initPhysics(phWorldRef,position);
}

// inits the physics at the current character position, used afte the initial
// state is set
void CharacterBase::initPhysics() {
    initPhysics(phWorldRef, mCharacterAnimation.getPosition());
    physicsInitialized = true;
}

void CharacterBase::loadTextures() {
    baseTextureManager.load(Textures::PhysicAssetsID::Empty,
                            "assets/scenes/99_dummy/box_empty.png");
}

void CharacterBase::destroyPhysics() { phWorldRef->DestroyBody(mBody); }

float CharacterBase::getDirectionMultiplier()
{
    return (getMoveDirection() == EMoveDirection::Left ? -1.0 : 1.0);
}


void CharacterBase::jumpForward()
{
    fmt::print("JumpForward: x:{} y:{}\n", mCharSettings.jumpForwardImpulse.x, mCharSettings.jumpForwardImpulse.y);

    b2MassData mass {.mass= mCharSettings.bodyMass
    , .center= mCharSettings.massCenter, .I=0};
    mBody->SetMassData(&mass);
    fmt::print("dirMultiplier: {}\n",getDirectionMultiplier());
    fmt::print("dirDirectionn: {}\n",static_cast<int>(getMoveDirection()));
    mBody->ApplyLinearImpulseToCenter( b2Vec2(mCharSettings.jumpForwardImpulse.x*getDirectionMultiplier()
                                      , mCharSettings.jumpForwardImpulse.y), true);
}


void CharacterBase::jumpUp() {
    fmt::print("JumpUp x:{} y: {}\n", mCharSettings.jumpUpImpulse.x, mCharSettings.jumpUpImpulse.y);

    // float impulse = mBody->GetMass() * 10000;
    b2MassData mass {.mass= mCharSettings.bodyMass
    , .center= mCharSettings.massCenter, .I=0};
    mBody->SetMassData(&mass);

    mBody->ApplyLinearImpulseToCenter(mCharSettings.jumpUpImpulse
                              , true);
    //mBody->ApplyLinearImpulseToCenter(mCharSettings.JumpUpForce,true);
}

void CharacterBase::onNotify(const SceneNode &entity, Ryu::EEvent event) {
    switch(event)
    {

        case Ryu::EEvent::TemporaryOutput:
        {
            mAnimationManager->outputStoredAnimations();
        mCharacterAnimation.setPosition(mCharacterAnimation.getPosition() + positionCrossOffset);
        }

        default: {}
    }

    // fmt::print("Called onNotify in CharacterBase\n");
    mCharacterState->onNotify(
        *this,
        event); // notify the state (we dont want the states to be an observer)
}

void CharacterBase::initPhysics(std::unique_ptr<b2World> &phWorld,
                                const sf::Vector2f &position) {
    // TODO: make it adjustable ? or remove and add new ? -> e.g. duck state ->
    // halfPhysics box
    // init physics after the charactersprite was created !
    // Create the body of the falling Crate
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody; /// TODO: or even kinematic body ?
    bodyDef.position.Set(Converter::pixelsToMeters<double>(position.x),
                         Converter::pixelsToMeters<double>(inDuckMode() ? position.y + (DUCK_FRAME_SIZE.second / 2) : position.y));
    bodyDef.fixedRotation = true;
    bodyDef.gravityScale = 4.8f;

    // Create a shape
    b2PolygonShape polygonShape;
    // TODO write convert functions Pixels<->meter (box2d) and reset
    // polygonshape wenn aniation changes
    // polygonShape.SetAsBox(mCharacterAnimation.getTexture()->getSize().x
    // / 20.f, mCharacterAnimation.getTexture()->getSize().y / 20.f ); /*
    // dimension.x/2.f,dimension.y/2.f */
    // polygonShape.SetAsBox(0.5,0.9);

    int size_x = 0; // mCharacterAnimation.getSprite().getTextureRect().width;
    int size_y = 0;

    if (inDuckMode()){
        size_x =
            DUCK_FRAME_SIZE
                .first; // mCharacterAnimation.getSprite().getTextureRect().width;
        size_y =
            DUCK_FRAME_SIZE
                .second; // mCharacterAnimation.getSprite().getTextureRect().height;
    } else {
        size_x =
            INIT_FRAME_SIZE
                .first; // mCharacterAnimation.getSprite().getTextureRect().width;
        size_y =
            INIT_FRAME_SIZE
                .second; // mCharacterAnimation.getSprite().getTextureRect().height;
    }

    polygonShape.SetAsBox(Converter::pixelsToMeters<double>(size_x * 0.5f),
                          Converter::pixelsToMeters<double>(size_y * 0.5f));

    // Create a fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &polygonShape;
    fixtureDef.density = 5.f;   /// for dynamic objects density needs to be > 0
    fixtureDef.friction = 0.1f; /// recommended by  b2d docu
    fixtureDef.restitution = 0.1;

    mBody = phWorld->CreateBody(&bodyDef);
    mFixture = mBody->CreateFixture(&fixtureDef);
    //fmt::print("Duckmode: {}\n", inDuckMode() ? " true " : " false ");
    sf::Shape *shape = new sf::RectangleShape(sf::Vector2f(size_x, size_y));
    shape->setOrigin(size_x / 2.0f, size_y / 2.0f);
    shape->setPosition(sf::Vector2f(position.x, position.y ));
    shape->setTexture(
        &baseTextureManager.getResource(Textures::PhysicAssetsID::Empty));

    mBody->GetUserData().pointer = (uintptr_t)shape;

    std::cout << "Init character at position "
              << Converter::metersToPixels(bodyDef.position.x) << ","
              << Converter::metersToPixels(bodyDef.position.y) << "\n";
    // mBody->SetLinearVelocity(b2Vec2(0.0f, -50.0f));
}

sf::Shape *
CharacterBase::getShapeFromCharPhysicsBody(b2Body *physicsBody) const {
    b2BodyUserData &data = physicsBody->GetUserData();
    sf::Shape *shape = reinterpret_cast<sf::RectangleShape *>(data.pointer);

    shape->setPosition(Converter::metersToPixels(physicsBody->GetPosition().x),
                       Converter::metersToPixels(physicsBody->GetPosition().y));
    shape->setRotation(Converter::radToDeg<double>(physicsBody->GetAngle()));
    return shape;
}

void CharacterBase::drawCurrent(sf::RenderTarget &target,
                                sf::RenderStates) const {
    // t.b.c
    if (mBody) {
        target.draw(*(getShapeFromCharPhysicsBody(mBody)));
    }
}

CharacterBase::~CharacterBase() {}

void CharacterBase::notifyObservers(Ryu::EEvent event) { notify(*this, event); }

std::unique_ptr<CharacterState> &CharacterBase::getCurrentCharacterState() {
    return mCharacterState;
}

void CharacterBase::handleInput(EInput input) {
    // if new state is created through the input we change the mCharacterState
    // to this
    std::unique_ptr<CharacterState> state =
        mCharacterState->handleInput(*this, input);

    if (state != nullptr) {
        mCharacterState->exit(*this);
        mCharacterState = std::move(state);
        mCharacterState->enter(*this);
    }
}

bool CharacterBase::allowedToFall() {
    return (mECharacterState._value != ECharacterState::JumpUp && mECharacterState._value != ECharacterState::JumpForward);
}

bool CharacterBase::inDuckMode() {
    return (mECharacterState._value == ECharacterState::DuckEnter
            || mECharacterState._value == ECharacterState::DuckEnd
            || mECharacterState._value == ECharacterState::DuckIdle
            || mECharacterState._value == ECharacterState::DuckWalk
    );
}

void CharacterBase::update(sf::Time deltaTime) {
    // std::cout //<< " x(pBody): " <<
    // Converter::metersToPixels(mBody->GetPosition().x)
    //<< " y(pBody): " << Converter::metersToPixels(mBody->GetPosition().y) <<
    //"\n"
    //          << " v " << mBody->GetLinearVelocity().x << " | " <<
    //          mBody->GetLinearVelocity().y << "\n length: " <<
    //          mBody->GetLinearVelocity().Length() << "\n";
    // TODO this has to be moved to a new state ! (falling)
    // dummy impl. / without falling animation
    if (mBody->GetLinearVelocity().y > 0.5f) {
        if (allowedToFall() && not mCharacterFalling &&
            mECharacterState._value != ECharacterState::Falling) {
            mCharacterFalling = true;
            std::unique_ptr<CharacterStateFalling> state =
                std::make_unique<CharacterStateFalling>();
            mCharacterState->exit(*this);
            mCharacterState = std::move(state);
            mCharacterState->enter(*this);
        }

        mCharacterAnimation.setPosition(
            Converter::metersToPixels<double>(mBody->GetPosition().x),
            inDuckMode()
            ? (Converter::metersToPixels<double>(mBody->GetPosition().y  - (DUCK_FRAME_SIZE.second)/2))
            : Converter::metersToPixels<double>(mBody->GetPosition().y));

    }
    /*
        fmt::print("GravityScale: {}\n",  mBody->GetGravityScale());;
        fmt::print("Inertia: {}\n",  mBody->GetInertia());
        fmt::print("Mass: {}\n",  mBody->GetMass());
    */
    if (not inDuckMode() && IsInBounds(mBody->GetLinearVelocity().y, 0.f, 0.01f)) {
        mCharacterFalling = false;
    }

    updateCharacterPosition(deltaTime);

    checkClimbingState();
}

void
CharacterBase::checkClimbingState()
{
    auto contacts = mBody->GetContactList();


}

void
CharacterBase::eraseRaycast(std::string rcName)
{
 rayCastPoints.erase(rcName);
}

void CharacterBase::toggleTestStates()
{
    // TODO: as these are on another sritesheet we need to change the textureatlas on the character
    // and setup fresh preferences for center/texturesize etc. / evtl. we need to change the character-
    // position (see reference point (cross in adventures when changing outfits / animations))
    if(testStateCurrent == testStatesCount) testStateCurrent = 0;

    if(testStateCurrent == 0)
    {
        changeState(std::make_unique<CharacterStateLedgeHang>());
    }
    if(testStateCurrent == 1)
    {
        changeState(std::make_unique<CharacterStateLedgeClimbUp>());
    }

    testStateCurrent++;

    mBody->Dump();

}

void CharacterBase::changeState(std::unique_ptr<CharacterState> toState) {
    mCharacterState->exit(*this);
    mCharacterState = std::move(toState);
    mCharacterState->enter(*this);
}

void CharacterBase::setupAnimation(Textures::CharacterID aniId) {
    RyuParser::Animation aniConfig;
    auto spritesheetId = AnimationToSpritesheetID.at(aniId);
    aniConfig =
        mAnimationManager->getCharacterAnimationConfig(spritesheetId, aniId);
    // fmt::print("startF(x): {}, startD(y): {}","bla","bla");
    AnimationConfiguration config{
        // TODO: check if x/y are in correct order for later spritesheets
        .frameSize = {aniConfig.frameSize.y, aniConfig.frameSize.x},
        .startFrame = {aniConfig.frames.at(0).x, aniConfig.frames.at(0).y},
        .numFrames = aniConfig.numFrames,
        .duration = aniConfig.animationDuration,
        .repeat = aniConfig.repeat,
        .animationId = aniId};
/*
    fmt::print("setupAnimation for CharacterId '{}'': StartFrame ({}/{}) \n ",
               aniId._to_string(), aniConfig.frames.at(0).x,
               aniConfig.frames.at(0).y);
*/

    getSpriteAnimation().setFrameSize(config.frameSize);
    getSpriteAnimation().setStartFrame(
        {config.startFrame.x, config.startFrame.y});
    getSpriteAnimation().setNumFrames(config.duration, aniConfig.frames);
    getSpriteAnimation().setDuration(config.duration);
    getSpriteAnimation().setRepeating(config.repeat);

    setTextureOnCharacter(spritesheetId);

    // set origin of texture to center
    sf::FloatRect bounds = getSpriteAnimation().getSprite().getLocalBounds();
    getSpriteAnimation().getSprite().setOrigin(bounds.width / 2.f,
                                               bounds.height / 2.f);

    // the first time we need to init physics-body etc
    if (not physicsInitialized) {
        initPhysics();
    }
}

/*
**
 */
void CharacterBase::updateCharacterPosition(sf::Time deltaTime) {
    mCharacterAnimation.update(deltaTime);

    //
    if (mECharacterState._value != ECharacterState::FallingEnd &&
        mECharacterState._value != ECharacterState::JumpUp &&
        mECharacterState._value != ECharacterState::JumpForward &&
        not mCharacterFalling) {

        if(not mSetOffset)
        {
            if(not IsInBounds(mLastBodyPosition.Length() - mBody->GetPosition().Length(),0.f, 0.01f))
            {
                mCharacterAnimation.move(movement * deltaTime.asSeconds());
            }
        }
        else
        {
            mCharacterAnimation.setPosition(mCharacterAnimation.getPosition());
            mSetOffset = false;
        }
            mBody->SetLinearVelocity(
            {mCharSettings.MoveMultiplierX *
                 Converter::pixelsToMeters<float>(movement.x),
             mCharSettings.MoveMultiplierY *
             Converter::pixelsToMeters<float>(movement.y)});
            mLastBodyPosition = mBody->GetPosition();
    }

    if(mECharacterState._value == ECharacterState::JumpUp ||mECharacterState._value == ECharacterState::JumpForward)
    {
        // physics body get a impulse in jump(), so here no update is needed
        auto pPosi = mBody->GetPosition();
        mCharacterAnimation.setPosition(Converter::metersToPixels(pPosi.x),
                                        Converter::metersToPixels(pPosi.y));
    }

    mCharacterState->update(*this);
}

void CharacterBase::setMovement(sf::Vector2f _movement) {
    movement = _movement;
}

void CharacterBase::setMoveDirection(EMoveDirection _movementDir) {
    mMoveDirection = _movementDir;
}

void CharacterBase::setTexture(
    AssetManager<sf::Texture, Textures::SpritesheetID> &textureManager,
    Textures::SpritesheetID id) {
    mCharacterAnimation.setTexture(textureManager.getResource(id));
}

void CharacterBase::setTextureOnCharacter(Textures::SpritesheetID textureId) {
    // TODO implement st here ?
}

void CharacterBase::changeColor(sf::Color color) {
    // mCharacterAnimationShape.setFillColor(color);
}

//} /// namespace ryu
