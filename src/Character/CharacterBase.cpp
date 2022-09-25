#include <Ryu/Character/CharacterBase.h>
#include <Ryu/Statemachine/CharacterStateIdle.h>
#include <Ryu/Core/AssetManager.h>

#include <iostream>
#include <memory>

using namespace ryu;

CharacterBase::CharacterBase() : 
    mCharacterSprite(),
    mCharacterState(new CharacterStateIdle()) /// TODO: smart pointer for states !!! ; beginn with IDLE
{

}

void
CharacterBase::loadTextures()
{}

CharacterBase::CharacterBase(ECharacterState startState)
: mECharacterState(startState)
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
CharacterBase::handleInput(EInput input)
{
    // if new state is cretead through the input we change the mCharacterState to this
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
    sf::Vector2f movement(0.f, 0.f);
	if(mIsMovingUp)
		movement.y -= PlayerSpeed;
	if(mIsMovingDown)
		movement.y += PlayerSpeed;
	if(mIsMovingLeft)
		movement.x -= PlayerSpeed;
	if(mIsMovingRight)
		movement.x += PlayerSpeed;

	mCharacterSprite.move(movement * deltaTime.asSeconds());
	//std::cout << std::to_string(movement.x) << "," << std::to_string(movement.y) << std::endl;

    mCharacterState->update(*this);
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
