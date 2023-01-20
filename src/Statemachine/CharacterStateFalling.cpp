#include <Ryu/Character/CharacterBase.h>
#include <Ryu/Statemachine/CharacterStateFalling.h>
#include <Ryu/Statemachine/CharacterStateIdle.h>
#include <Ryu/Statemachine/CharacterState.h>
#include <Ryu/Core/Utilities.h>

#include <iostream>
#include <memory>
#include <type_traits>
#include <typeinfo>


//namespace ryu{

CharacterStateFalling::CharacterStateFalling()
    :  touchedFloor(false)
     , timerTimeInMs(0.0f)
{
    std::cout << "Falling-cdor\n";
}

CharacterStateFalling::~CharacterStateFalling(){
    std::cout << "Falling-dtor\n";
}

std::unique_ptr<CharacterState>
CharacterStateFalling::handleInput(CharacterBase& character,EInput input)
{
    switch (input)
    {
       case EInput::PressDown:
       {
           // TODO: Fall down fast ?
           //return new CharacterStateRun();    
       }
     
    default:
        break;
    }

    return nullptr;
}
     
void 
CharacterStateFalling::update(CharacterBase& character)
{
    auto elTime = timer.getElapsedTime().asMilliseconds();
    std::cout << elTime << std::endl;
    if(not character.isFalling() && (elTime > timerTimeInMs ))// TODO:test mit neuem eigenem CharState
    {
        std::unique_ptr<CharacterStateIdle> state = std::make_unique<CharacterStateIdle>();
        character.changeState(std::move(state));
    }

    if(elTime == 0)
    {
      auto rc = RyuPhysics::createRaycast("below",std::make_pair(character.getSpriteAnimation().getPosition().x,character.getSpriteAnimation().getPosition().y+RyuPhysics::raycastOffset),90,60.0f,character.getMoveDirection(),character.getPhysicsWorldRef(),character.rayCastPoints);
      
      if(rc.m_Hit)
      {
        std::cout << "Boom\n";
        touchFloor(character); 
      }
    }
}

void
CharacterStateFalling::touchFloor(CharacterBase& character)
{
    if(not touchedFloor)
    {
        touchedFloor = true;
        timer.restart();
        std::cout << "touched floor\n";
        character.setupAnimation({
            .frameSize={80,96}
           ,.startFrame={0,4}
           ,.numFrames=11
           ,.duration = sf::milliseconds(1500)
           ,.repeat = true
           ,.animationId = Textures::CharacterID::IchiEndFallingLow});
        timerTimeInMs = 2000;
    }
}


void
CharacterStateFalling::enter(CharacterBase& character)
{
    character.setupAnimation({
            .frameSize={80,96}
           ,.startFrame={3,2}
           ,.numFrames=4
           ,.duration = sf::seconds(1)
           ,.repeat = true
           ,.animationId = Textures::CharacterID::IchiFallingLow});

}

void
CharacterStateFalling::exit(CharacterBase& character)
{
    std::cout << "delete below\n";
    character.rayCastPoints.erase("below");
}

//} /// namespace ryu