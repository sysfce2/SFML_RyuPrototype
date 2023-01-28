#include <Ryu/Character/CharacterBase.h>
#include <Ryu/Statemachine/CharacterStateIdle.h>
#include <Ryu/Statemachine/CharacterStateJumpUp.h>
#include <Ryu/Statemachine/CharacterStateDuckEnter.h>
#include <Ryu/Statemachine/CharacterStateRun.h>
#include <Ryu/Statemachine/CharacterState.h>

#include <iostream>
#include <memory>
#include <type_traits>
#include <typeinfo>


//namespace ryu{

CharacterStateIdle::CharacterStateIdle(){}

CharacterStateIdle::~CharacterStateIdle(){}

std::unique_ptr<CharacterState>
CharacterStateIdle::handleInput(CharacterBase& character,EInput input)
{
    switch (input)
    {
       case EInput::PressLeft:
       case EInput::PressRight:
       {
           return std::move(std::make_unique<CharacterStateRun>());    
       }

       case EInput::PressUp:
       {
           return std::move(std::make_unique<CharacterStateJumpUp>());    
       }

       case EInput::PressDown:
       {
           return std::move(std::make_unique<CharacterStateDuckEnter>());    
       }
     
    default:
        break;
    }

    return nullptr;
}
     
void 
CharacterStateIdle::update(CharacterBase& character)
{
    //std::cout << "IDLE"<< std::endl;
}


void
CharacterStateIdle::enter(CharacterBase& character)
{
    character.setupAnimation({
                .frameSize={80,96}
               ,.startFrame={0,0}
               ,.numFrames=1
               ,.duration = sf::seconds(1)
               ,.repeat = true
               ,.animationId = Textures::CharacterID::IchiIdleRun});

    character.setCharacterStateEnum(ECharacterState::Idle);
}

void
CharacterStateIdle::exit(CharacterBase& character)
{
    character.getSpriteAnimation().restart();
}

//} /// namespace ryu