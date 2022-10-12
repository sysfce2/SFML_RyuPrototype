#include <Ryu/Character/CharacterBase.h>
#include <Ryu/Statemachine/CharacterStateIdle.h>
#include <Ryu/Core/AssetManager.h>

#include <iostream>
#include <memory>

//namespace ryu {

CharacterBase::CharacterBase() 
    :mCharacterAnimation()
    ,mCharacterState(std::make_unique<CharacterStateIdle>())
    ,movement(0.f,0.f)
    ,mMoveDirection(EMoveDirecton::Right)
{}

CharacterBase::CharacterBase(ECharacterState startState)
    :mECharacterState(startState)
    ,mCharacterSpeed(55.0f) // startvalue playerspeed
    ,mMoveDirection(EMoveDirecton::Right)
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
