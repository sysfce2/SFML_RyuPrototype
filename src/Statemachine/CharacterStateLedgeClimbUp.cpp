#include <Ryu/Statemachine/CharacterStateLedgeClimbUp.h>

#include <Ryu/Character/CharacterBase.h>
#include <Ryu/Statemachine/CharacterStateIdle.h>
#include <Ryu/Statemachine/CharacterState.h>

#include <memory>


//namespace ryu{

CharacterStateLedgeClimbUp::CharacterStateLedgeClimbUp(){}

CharacterStateLedgeClimbUp::~CharacterStateLedgeClimbUp(){}

std::unique_ptr<CharacterState>
CharacterStateLedgeClimbUp::handleInput(CharacterBase& character,EInput input)
{
    switch (input)
    {
       case EInput::PressLeft:
       case EInput::PressRight:
       {
       }

       case EInput::PressUp:
       {
       }

       case EInput::PressDown:
       {
           //return std::move(std::make_unique<CharacterStateIdle>());
       }

    default:
        break;
    }

    return nullptr;
}

void
CharacterStateLedgeClimbUp::update(CharacterBase& character)
{
    if (character.getSpriteAnimation().isFinished()) {
        std::unique_ptr<CharacterStateIdle> state =
            std::make_unique<CharacterStateIdle>();
        character.changeState(std::move(state));
    }
}


void
CharacterStateLedgeClimbUp::enter(CharacterBase& character)
{
    character.setupAnimation(Textures::CharacterID::IchiLedgeClimbUp);
    character.setCharacterStateEnum(ECharacterState::ClimbUp);
}

void
CharacterStateLedgeClimbUp::exit(CharacterBase& character)
{
    character.getSpriteAnimation().restart();
}

//} /// namespace ryu
