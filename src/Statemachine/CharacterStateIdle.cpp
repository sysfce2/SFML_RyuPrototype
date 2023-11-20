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
}


void
CharacterStateIdle::enter(CharacterBase& character)
{
    character.setupAnimation(Textures::CharacterID::IchiIdle);
    character.setCharacterStateEnum(ECharacterState::Idle);

    if(character.duckStateActive())
    {
        character.destroyPhysics();
        character.initPhysics();
        character.setDuckState(false);
    }
}

void
CharacterStateIdle::exit(CharacterBase& character)
{
    character.getSpriteAnimation().restart();
}

//} /// namespace ryu
