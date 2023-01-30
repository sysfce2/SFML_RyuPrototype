#include <Ryu/Character/CharacterBase.h>
#include <Ryu/Statemachine/CharacterStateJumpUp.h>
#include <Ryu/Statemachine/CharacterStateDuckEnter.h>
#include <Ryu/Statemachine/CharacterStateIdle.h>
#include <Ryu/Statemachine/CharacterState.h>

#include <iostream>
#include <memory>
#include <type_traits>
#include <typeinfo>


//namespace ryu{

CharacterStateJumpUp::CharacterStateJumpUp()
{}

CharacterStateJumpUp::~CharacterStateJumpUp(){}

std::unique_ptr<CharacterState>
CharacterStateJumpUp::handleInput(CharacterBase& character,EInput input)
{
    switch (input)
    {
       case EInput::PressLeft:
       case EInput::PressRight:
       case EInput::PressUp:
       case EInput::PressDown:
       {
                
           return nullptr;
       }
    default:
        break;
    }

    return nullptr;
}
     
void 
CharacterStateJumpUp::update(CharacterBase& character)
{

    if(character.getSpriteAnimation().isFinished())
    {
        std::unique_ptr<CharacterStateIdle> state = std::make_unique<CharacterStateIdle>();
        character.changeState(std::move(state));
    }
}


void
CharacterStateJumpUp::enter(CharacterBase& character)
{
    character.setupAnimation({
                .frameSize={80,96}
               ,.startFrame={10,5}
               ,.numFrames=14
               ,.duration = sf::seconds(1)
               ,.repeat = false
               ,.animationId = Textures::CharacterID::IchiJumpUp});

    character.setCharacterStateEnum(ECharacterState::JumpUp);
    character.jumpUp();
}

void
CharacterStateJumpUp::exit(CharacterBase& character)

{
    character.getSpriteAnimation().restart();
}

//} /// namespace ryu