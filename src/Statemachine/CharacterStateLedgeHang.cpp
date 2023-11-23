#include <Ryu/Statemachine/CharacterStateLedgeClimbUp.h>
#include <Ryu/Statemachine/CharacterStateLedgeHang.h>


#include <Ryu/Character/CharacterBase.h>
#include <Ryu/Statemachine/CharacterStateIdle.h>
#include <Ryu/Statemachine/CharacterState.h>

#include <SFML/System/Vector2.hpp>
#include <memory>


//namespace ryu{

CharacterStateLedgeHang::CharacterStateLedgeHang(){}

CharacterStateLedgeHang::~CharacterStateLedgeHang(){}

std::unique_ptr<CharacterState>
CharacterStateLedgeHang::handleInput(CharacterBase& character,EInput input)
{
    switch (input)
    {
       case EInput::PressLeft:
       case EInput::PressRight:
       {
           break;
       }

       case EInput::PressUp:
       {
           return std::move(std::make_unique<CharacterStateLedgeClimbUp>());
       }

       case EInput::PressDown:
       {
           return std::move(std::make_unique<CharacterStateIdle>());
       }

    default:
        break;
    }

    return nullptr;
}

void
CharacterStateLedgeHang::update(CharacterBase& character)
{
}


void
CharacterStateLedgeHang::enter(CharacterBase& character)
{
    //TODO: temporary -> this should be automatically added !
    character.getSpriteAnimation().setOrigin(9,88);
    character.setupAnimation(Textures::CharacterID::IchiLedgeHangIdle);
    character.setCharacterStateEnum(ECharacterState::Hanging);
}

void
CharacterStateLedgeHang::exit(CharacterBase& character)
{
    character.getSpriteAnimation().restart();
}

//} /// namespace ryu
