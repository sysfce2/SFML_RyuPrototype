#include <Ryu/Statemachine/CharacterStateRun.h>
#include <Ryu/Statemachine/CharacterStateIdle.h>
#include <Ryu/Character/CharacterBase.h>
#include <Ryu/Character/CharacterIchi.h>
#include <Ryu/Events/EventEnums.h>

#include <iostream>
#include <memory>

//namespace ryu{

CharacterStateRun::CharacterStateRun()
: mRunCharacterSpeed(150.f)
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
    switch (input)
    {
       case EInput::ReleaseLeft:
       case EInput::ReleaseRight:
       case EInput::ReleaseUp:
       case EInput::ReleaseDown:
       {
           return std::move(std::make_unique<CharacterStateIdle>());    
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
    
}

void
CharacterStateRun::enter(CharacterBase& character)
{
    character.changeColor(sf::Color::Green);
    character.setTextureOnCharacter(Textures::CharacterID::IchiKatanaWalk);

    mLastCharacterSpeed = character.getCharacterSpeed();
    character.setCharacterSpeed(mRunCharacterSpeed);
    character.notifyObservers(Event::CharacterSpeedChanged);
}

void
CharacterStateRun::exit(CharacterBase& character)
{
    character.setCharacterSpeed(mLastCharacterSpeed);
    character.notifyObservers(Event::CharacterSpeedChanged);
}

//} /// namespace ryu