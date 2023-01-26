#include <Ryu/Statemachine/CharacterStateDuckWalk.h>
#include <Ryu/Statemachine/CharacterStateDuckIdle.h>
#include <Ryu/Statemachine/CharacterStateIdle.h>
#include <Ryu/Character/CharacterBase.h>
#include <Ryu/Statemachine/CharacterState.h>
#include <Ryu/Core/AssetIdentifiers.h>

#include <iostream>
#include <memory>


//namespace ryu{

CharacterStateDuckWalk::CharacterStateDuckWalk(){}

CharacterStateDuckWalk::~CharacterStateDuckWalk(){}

std::unique_ptr<CharacterState>
CharacterStateDuckWalk::handleInput(CharacterBase& character,EInput input)
{
    switch (input)
    {
       case EInput::ReleaseLeft:
       case EInput::ReleaseRight:
       {
           return std::move(std::make_unique<CharacterStateDuckIdle>());
       }
       case EInput::PressUp:
       {
           return std::move(std::make_unique<CharacterStateIdle>());
       }
     
    default:
        break;
    }

    return nullptr;
}
     
void 
CharacterStateDuckWalk::update(CharacterBase& character)
{
    //std::cout << "IDLE"<< std::endl;
}


void
CharacterStateDuckWalk::enter(CharacterBase& character)
{
    
    character.setupAnimation({
                .frameSize={80,96}
                ,.startFrame={10,3}
                ,.numFrames=7
                ,.duration = sf::seconds(1)
                ,.repeat = true
                ,.animationId = Textures::CharacterID::IchiDuckWalk});

    character.setCharacterStateEnum(ECharacterState::DuckWalk);
}

void
CharacterStateDuckWalk::exit(CharacterBase& character)
{
    character.getSpriteAnimation().restart();
}

//} /// namespace ryu