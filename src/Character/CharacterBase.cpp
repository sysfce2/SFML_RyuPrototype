#include <Ryu/Character/CharacterBase.h>
#include <Ryu/Statemachine/CharacterStateIdle.h>
#include <Ryu/Core/AssetManager.h>

#include <box2d/box2d.h>
#include <Thirdparty/glm/glm.hpp>

#include <iostream>
#include <memory>

//namespace ryu {

CharacterBase::CharacterBase(std::unique_ptr<b2World>& phWorld,  
                            const glm::vec2 &position) 
    :mCharacterAnimation()
    ,mCharacterState(std::make_unique<CharacterStateIdle>())
    ,movement(0.f,0.f)
    ,mMoveDirection(EMoveDirecton::Right)
    ,phWorldRef(phWorld)
{
   //initPhysics(phWorld,position); 
}

CharacterBase::CharacterBase(ECharacterState startState, 
                            std::unique_ptr<b2World>& phWorld,  
                            const glm::vec2 &position)
    :mECharacterState(startState)
    ,mCharacterSpeed(55.0f) // startvalue playerspeed
    ,mMoveDirection(EMoveDirecton::Right)
    ,phWorldRef(phWorld)
{
   switch(mECharacterState)
   {
       case ECharacterState::Idle:
        mCharacterState = std::make_unique<CharacterStateIdle>();
        break;
       default:
        mCharacterState = std::make_unique<CharacterStateIdle>();
   } 
}

void
CharacterBase::initPhysics(const glm::vec2 &position)
{
    initPhysics(phWorldRef,position);
}


void
CharacterBase::initPhysics(std::unique_ptr<b2World>& phWorld,  const glm::vec2 &position)
{
    // nit physics after the charactersprite was created !
    // Create the body of the falling Crate
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position.x,position.y);

    mBody = phWorld->CreateBody(&bodyDef);

    // Create a shape
    b2PolygonShape polygonShape;
    // TODO write convert functions Pixels<->meter (box2d) and reset polygonshape wenn aniation changes
    // polygonShape.SetAsBox(mCharacterAnimation.getTexture()->getSize().x / 20.f, mCharacterAnimation.getTexture()->getSize().y / 20.f ); /* dimension.x/2.f,dimension.y/2.f */
    polygonShape.SetAsBox(0.5,0.9);

    polygonShape.SetAsBox(mCharacterAnimation.getSprite().getTextureRect().width * 0.5f - b2_polygonRadius, mCharacterAnimation.getSprite().getTextureRect().width * 0.5f - b2_polygonRadius);

    // Create a fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &polygonShape;
    fixtureDef.density = 20.f; /// for dynamic objects density needs to be > 0
    fixtureDef.friction = 0.3f; /// recommended by b2d docu 
    mFixture = mBody->CreateFixture(&fixtureDef);
    std::cout << "Init character at position "<< bodyDef.position.x << "," << bodyDef.position.y << "\n";
    mBody->SetLinearVelocity(b2Vec2(0.0f, -50.0f));


}

void
CharacterBase::loadTextures()
{}


CharacterBase::~CharacterBase() {}

void 
CharacterBase::notifyObservers(Event event)
{   
    notify(*this,event);
}

std::unique_ptr<CharacterState>& 
CharacterBase::getCurrentCharacterState()
{
    return mCharacterState;
}

void 
CharacterBase::handleInput(EInput input)
{
    // if new state is created through the input we change the mCharacterState to this
    std::unique_ptr<CharacterState> state = mCharacterState->handleInput(*this,input);

    if(state != nullptr)
    {
        mCharacterState->exit(*this);
        mCharacterState = std::move(state);
        mCharacterState->enter(*this);
    }
}
 
void
CharacterBase::update(sf::Time deltaTime)
{
    updateCharacterState(deltaTime);
}

void
CharacterBase::updateCharacterState(sf::Time deltaTime)
{
    mCharacterAnimation.update(deltaTime);
    mCharacterAnimation.move(movement * deltaTime.asSeconds());
   
    mCharacterState->update(*this);
}

 void 
 CharacterBase::setMovement(sf::Vector2f _movement)
 {
     movement = _movement;
 }

 void 
 CharacterBase::setMoveDirection(EMoveDirecton _movementDir)
 {
     mMoveDirection = _movementDir;
 }

void 
CharacterBase::setTexture(AssetManager<sf::Texture, Textures::CharacterID> &textureManager, Textures::CharacterID id)
{
    mCharacterAnimation.setTexture(textureManager.getResource(id));
   
}

void
CharacterBase::setTextureOnCharacter(Textures::CharacterID textureId)
{
    // TODO implement st here ?
    
}

void
CharacterBase::changeColor(sf::Color color)
{
    //mCharacterAnimationShape.setFillColor(color);
}

//} /// namespace ryu
