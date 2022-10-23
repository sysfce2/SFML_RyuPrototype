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
{}

void
CharacterStateRun::enter(CharacterBase& character)
{
    //character.setTextureOnCharacter(Textures::CharacterID::IchiKatanaWalk);
    character.setTextureOnCharacter(Textures::CharacterID::IchiIdleRun);

    mLastCharacterSpeed = character.getCharacterSpeed();
    character.setCharacterSpeed(mRunCharacterSpeed);
    character.notifyObservers(Event::CharacterSpeedChanged);

    // TODO: animation spec tospecial class / manager ?
    character.getSpriteAnimation().setFrameSize(sf::Vector2i(80,96));
    character.getSpriteAnimation().setStartFrame({80,0});
    character.getSpriteAnimation().setNumFrames(8);
    character.getSpriteAnimation().setDuration(sf::seconds(1));
    character.getSpriteAnimation().setRepeating(true);

    sf::FloatRect bounds = character.getSpriteAnimation().getSprite().getLocalBounds();
    character.getSpriteAnimation().getSprite().setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    //mSprite.setOrigin(120.f, 144.f);
    std::cout << "Boundswidth: " << bounds.width << "Boundsheight: " << bounds.height << "\n";
    //character.initPhysics(phWorld);
}

void
CharacterStateRun::exit(CharacterBase& character)
{
    character.setCharacterSpeed(mLastCharacterSpeed);
    character.notifyObservers(Event::CharacterSpeedChanged);

    character.getSpriteAnimation().setNumFrames(1);
    character.getSpriteAnimation().restart();
}

//} /// namespace ryu