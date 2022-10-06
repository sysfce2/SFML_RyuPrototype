#include <Ryu/Character/CharacterBase.h>
#include <Ryu/Statemachine/CharacterStateIdle.h>
#include <Ryu/Core/AssetManager.h>

#include <iostream>
#include <memory>

//namespace ryu {

CharacterBase::CharacterBase() : 
    mCharacterSprite()
    ,mCharacterState(std::make_unique<CharacterStateIdle>())
    ,movement(0.f,0.f)
{}

void
CharacterBase::loadTextures()
{}

CharacterBase::CharacterBase(ECharacterState startState)
: mECharacterState(startState)
, mCharacterSpeed(55.0f) // startvalue playerspeed
{
   switch(mECharacterState)
   {
       case ECharacterState::Idle:
        //mCharacterState = new CharacterStateIdle();
        mCharacterState = std::make_unique<CharacterStateIdle>();
        break;
       default:
        mCharacterState = std::make_unique<CharacterStateIdle>();
        //mCharacterState = new CharacterStateIdle();
   } 
}

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
    /*
    sf::Vector2f movement(0.f, 0.f);
	if(mIsMovingUp)
		movement.y -= PlayerSpeed;
	if(mIsMovingDown)
		movement.y += PlayerSpeed;
	if(mIsMovingLeft)
		movement.x -= PlayerSpeed;
	if(mIsMovingRight)
		movement.x += PlayerSpeed;
    */
   
	//std::cout << std::to_string(movement.x) << "," << std::to_string(movement.y) << std::endl;
	mCharacterSprite.move(movement * deltaTime.asSeconds());

    mCharacterState->update(*this);
}

 void 
 CharacterBase::setMovement(sf::Vector2f _movement)
 {
     movement = _movement;
 }

void 
CharacterBase::setTexture(AssetManager<sf::Texture, Textures::CharacterID> &textureManager, Textures::CharacterID id)
{
    mCharacterSprite.setTexture(textureManager.getResource(id));    
}

void
CharacterBase::setTextureOnCharacter(Textures::CharacterID textureId)
{
    // TODO implement st here ?
}

void
CharacterBase::changeColor(sf::Color color)
{
    //mCharacterSpriteShape.setFillColor(color);
}

//} /// namespace ryu
