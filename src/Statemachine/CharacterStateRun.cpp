#include <Ryu/Statemachine/CharacterStateRun.h>
#include <Ryu/Statemachine/CharacterStateIdle.h>
#include <Ryu/Character/CharacterBase.h>

#include <iostream>

using namespace ryu;

CharacterStateRun::CharacterStateRun(){}

CharacterStateRun::~CharacterStateRun(){}

CharacterState* 
CharacterStateRun::handleInput(CharacterBase& character,EInput input)
{
    switch (input)
    {
       case EInput::RELEASELEFT:
       case EInput::RELEASERIGHT:
       case EInput::RELEASEUP:
       case EInput::RELEASEDOWN:
       {
           return new CharacterStateIdle();    
       }
       break;
     
    default:
        break;
    }

    return nullptr;
}
     
void 
CharacterStateRun::update(CharacterBase& character)
{
    //std::cout << "RUN"<< std::endl;
}

void
CharacterStateRun::enter(CharacterBase& character)
{
    character.changeColor(sf::Color::Green);
    character.setTextureOnCharacter(Textures::CharacterID::IchiIdleRun);
}

void
CharacterStateRun::exit(CharacterBase& character)
{
    character.changeColor(sf::Color::Magenta);
}