#include <Ryu/Character/CharacterBase.h>
#include <Ryu/Statemachine/CharacterStateIdle.h>
#include <Ryu/Core/AssetManager.h>

using namespace ryu;

CharacterBase::CharacterBase() : 
    mPlayer(),
    characterState(new CharacterStateIdle()) /// TODO: smart pointer for states !!! ; beginn with IDLE
{
    /*
    mPlayerShape.setRadius(40.f);
	mPlayerShape.setPosition(100.f, 100.f);
	mPlayerShape.setFillColor(sf::Color::Cyan);
    */
}

CharacterBase::~CharacterBase() {}

void 
CharacterBase::handleInput(EInput input)
{
    // if new state is cretead through the input we change the characterstate to this
    // TODO: smartpointers !!!
    CharacterState* state = characterState->handleInput(*this,input);

    if(state != nullptr)
    {
        characterState->exit(*this);
        delete characterState;
        characterState = state;
        characterState->enter(*this);
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

	mPlayer.move(movement * deltaTime.asSeconds());
	//std::cout << std::to_string(movement.x) << "," << std::to_string(movement.y) << std::endl;

    characterState->update(*this);
}

void 
CharacterBase::setTexture(AssetManager<sf::Texture, Textures::CharacterID> &textureManager, Textures::CharacterID id)
{
    mPlayer.setTexture(textureManager.getResource(id));    
}

void
CharacterBase::changeColor(sf::Color color)
{
    //mPlayerShape.setFillColor(color);
}
