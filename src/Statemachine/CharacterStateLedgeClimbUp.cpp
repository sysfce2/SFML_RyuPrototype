#include "Ryu/Core/Utilities.h"
#include <Ryu/Statemachine/CharacterStateLedgeClimbUp.h>

#include <Ryu/Character/CharacterBase.h>
#include <Ryu/Statemachine/CharacterStateIdle.h>
#include <Ryu/Statemachine/CharacterState.h>

#include <SFML/System/Vector2.hpp>
#include <box2d/b2_math.h>
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
        b2Vec2 newBodyPos{Converter::pixelsToMeters<float>(character.getPositionCross().x),
                          Converter::pixelsToMeters<float>(character.getPositionCross().y)};
        character.getBody()->SetTransform((character.getBody()->GetPosition() + newBodyPos), 0.f);
        std::unique_ptr<CharacterStateIdle> state =
            std::make_unique<CharacterStateIdle>();
        character.changeState(std::move(state));
    }
}


void
CharacterStateLedgeClimbUp::enter(CharacterBase& character)
{
    // TODO: tmp:
    character.getSpriteAnimation().setOrigin(0,40);
    character.setupAnimation(Textures::CharacterID::IchiLedgeClimbUp);
    character.setCharacterStateEnum(ECharacterState::ClimbUp);
}

void
CharacterStateLedgeClimbUp::exit(CharacterBase& character)
{
    character.getSpriteAnimation().restart();
}

//} /// namespace ryu
