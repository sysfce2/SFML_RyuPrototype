#include <Ryu/Character/CharacterBase.h>
#include <Ryu/Statemachine/CharacterStateDuckIdle.h>
#include <Ryu/Statemachine/CharacterStateIdle.h>
#include <Ryu/Statemachine/CharacterStateDuckWalk.h>
#include <Ryu/Statemachine/CharacterStateRun.h>
#include <Ryu/Statemachine/CharacterState.h>

#include <iostream>
#include <memory>
#include <type_traits>
#include <typeinfo>


//namespace ryu{

CharacterStateDuckIdle::CharacterStateDuckIdle():idleSet(false){}

CharacterStateDuckIdle::~CharacterStateDuckIdle(){}

std::unique_ptr<CharacterState>
CharacterStateDuckIdle::handleInput(CharacterBase& character,EInput input)
{
    switch (input)
    {
       case EInput::PressLeft:
       case EInput::PressRight:
       {
           return std::move(std::make_unique<CharacterStateDuckWalk>());    
       }

       case EInput::PressUp:
       {
           return std::move(std::make_unique<CharacterStateIdle>());    
       }
       case EInput::PressDown:
            break;
     
    default:
        break;
    }

    return nullptr;
}
     
void 
CharacterStateDuckIdle::update(CharacterBase& character)
{
}


void
CharacterStateDuckIdle::enter(CharacterBase& character)
{
    character.setupAnimation({
                .frameSize={80,96}
               ,.startFrame={9,3}
               ,.numFrames=1
               ,.duration = sf::milliseconds(500)
               ,.repeat = true
               ,.animationId = Textures::CharacterID::IchiDuckIdle});

    character.setCharacterStateEnum(ECharacterState::DuckIdle);
}

void
CharacterStateDuckIdle::exit(CharacterBase& character)
{
    character.getSpriteAnimation().restart();
}

//} /// namespace ryu