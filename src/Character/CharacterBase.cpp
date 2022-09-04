#include <Ryu/Character/CharacterBase.h>
#include <Ryu/Statemachine/CharacterStateIdle.h>

using namespace ryu;

CharacterBase::CharacterBase() : 
    mPlayerShape(),
    characterState(new CharacterStateIdle()) /// TODO: smart pointer for states !!! ; beginn with IDLE
{
    mPlayerShape.setRadius(40.f);
	mPlayerShape.setPosition(100.f, 100.f);
	mPlayerShape.setFillColor(sf::Color::Cyan);
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

	mPlayerShape.move(movement * deltaTime.asSeconds());
	//std::cout << std::to_string(movement.x) << "," << std::to_string(movement.y) << std::endl;

    characterState->update(*this);
}

void
CharacterBase::changeColor(sf::Color color)
{
    mPlayerShape.setFillColor(color);
}
