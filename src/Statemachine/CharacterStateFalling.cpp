#include <Ryu/Character/CharacterBase.h>
#include <Ryu/Statemachine/CharacterStateFalling.h>
#include <Ryu/Statemachine/CharacterStateFallingEnd.h>
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
    if(not touchedFloor)
    {
      auto rc = RyuPhysics::createRaycast(
            "below"
            ,std::make_pair(character.getSpriteAnimation().getPosition().x
            ,character.getSpriteAnimation().getPosition().y+RyuPhysics::raycastOffset)
            ,90,55.0f /// angle, length 
            ,character.getMoveDirection()
            ,character.getPhysicsWorldRef()
            ,character.rayCastPoints
            );
      
      if(rc.m_Hit)
      {
        std::cout << "Boom\n";
        touchedFloor = true;
        character.rayCastPoints.erase("below");
        std::unique_ptr<CharacterStateFallingEnd> state = std::make_unique<CharacterStateFallingEnd>();
        character.changeState(std::move(state));
      }
    }
}

void
CharacterStateFalling::enter(CharacterBase& character)
{
    character.setupAnimation({
            .frameSize={80,96}
           ,.startFrame={3,2}
           ,.numFrames=4
           ,.duration = sf::milliseconds(1000)
           ,.repeat = true
           ,.animationId = Textures::CharacterID::IchiFallingLow});
    character.setCharacterStateEnum(ECharacterState::Falling);
}

void
CharacterStateFalling::exit(CharacterBase& character)
{
    std::cout << "delete below\n";
    character.rayCastPoints.erase("below");
    character.getSpriteAnimation().restart();
}

//} /// namespace ryu