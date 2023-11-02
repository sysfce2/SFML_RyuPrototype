#include "Ryu/Statemachine/CharacterStateJumpNormal.h"
#include <Ryu/Statemachine/CharacterStateRun.h>
#include <Ryu/Statemachine/CharacterStateIdle.h>
#include <Ryu/Character/CharacterBase.h>
#include <Ryu/Character/CharacterIchi.h>
#include <Ryu/Events/EventEnums.h>

#include <iostream>
#include <memory>

//namespace ryu{

CharacterStateRun::CharacterStateRun()
: mRunCharacterSpeed(180.f)
{
    std::cout << "RunStateCreated" << std::endl;
}

CharacterStateRun::~CharacterStateRun()
{
    std::cout << "RunStateDestroyed" << std::endl;
}

std::unique_ptr<CharacterState> 
CharacterStateRun::handleInput(CharacterBase& character,EInput input)
{
    if(character.isFalling())
    {
        return nullptr;
    }
    
    switch (input)
    {
       case EInput::ReleaseLeft:
       case EInput::ReleaseRight:
       {
           return std::move(std::make_unique<CharacterStateIdle>());    
       }
        case EInput::PressUp:
        {
           return std::move(std::make_unique<CharacterStateJumpNormal>());
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
    if(character.isFalling())
    {

    }
}

void
CharacterStateRun::enter(CharacterBase& character)
{
    character.setupAnimation(Textures::CharacterID::IchiRun);

    mLastCharacterSpeed = character.getCharacterSpeed();
    character.setCharacterSpeed(mRunCharacterSpeed);
    character.notifyObservers(Ryu::EEvent::CharacterSpeedChanged);
    character.setCharacterStateEnum(ECharacterState::Run);

}

void
CharacterStateRun::exit(CharacterBase& character)
{
    character.setCharacterSpeed(mLastCharacterSpeed);
    character.notifyObservers(Ryu::EEvent::CharacterSpeedChanged);

    character.getSpriteAnimation().restart();
}

//} /// namespace ryu
