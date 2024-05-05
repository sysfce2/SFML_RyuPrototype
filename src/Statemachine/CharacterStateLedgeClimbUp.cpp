#include "Ryu/Control/CharacterEnums.h"
#include "Ryu/Core/Utilities.h"
#include <Ryu/Statemachine/CharacterStateLedgeClimbUp.h>

#include <Ryu/Character/CharacterBase.h>
#include <Ryu/Statemachine/CharacterStateIdle.h>
#include <Ryu/Statemachine/CharacterState.h>

#include <SFML/System/Vector2.hpp>
#include <box2d/b2_math.h>
#include <fmt/core.h>
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

       case EInput::ActionInput:
       {
       }

    default:
        break;
    }

    return nullptr;
}

void
CharacterStateLedgeClimbUp::update(CharacterBase& character)
{
    fmt::print("Current frame: {}\n",character.getSpriteAnimation().getCurrentFrame());
    if (character.getSpriteAnimation().getCurrentFrame() == (character.getSpriteAnimation().getFramesCount())) {
///*
        b2Vec2 newBodyPos{Converter::pixelsToMeters<float>(character.getPositionCross().x),
                          Converter::pixelsToMeters<float>(character.getPositionCross().y)};
//*/
        //character.getBody()->SetTransform((character.getBody()->GetPosition() + newBodyPos), 0.f);
        //character.getSpriteAnimation().restart();
        // TODO: check ob das mit der Zeit der animation zusammenaengt ? -> muessen das multiple von frametime sein
        // - check fps and set aniframetimes to multiples from fps time
        character.changeState(std::move(std::make_unique<CharacterStateIdle>()));
        //return;
    }
}


void
CharacterStateLedgeClimbUp::enter(CharacterBase& character)
{
    // TODO: tmp:
    //character.getSpriteAnimation().setOrigin(0,40);
    character.setupAnimation(Textures::CharacterID::IchiLedgeClimbUp);
    //character.setupAnimation(Textures::CharacterID::IchiLandLow);
    character.setCharacterStateEnum(ECharacterState::ClimbUp);
}

void
CharacterStateLedgeClimbUp::exit(CharacterBase& character)
{
    character.getSpriteAnimation().restart();
    fmt::print("exit ledgeclimb");
}

//} /// namespace ryu
