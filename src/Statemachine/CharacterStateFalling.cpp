#include <Ryu/Character/CharacterBase.h>
#include <Ryu/Statemachine/CharacterStateFalling.h>
#include <Ryu/Statemachine/CharacterStateIdle.h>
#include <Ryu/Statemachine/CharacterState.h>

#include <iostream>
#include <memory>
#include <type_traits>
#include <typeinfo>


//namespace ryu{

CharacterStateFalling::CharacterStateFalling(){}

CharacterStateFalling::~CharacterStateFalling(){}

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
    if(not character.isFalling())
    {
        std::unique_ptr<CharacterStateIdle> state = std::make_unique<CharacterStateIdle>();
        character.changeState(std::move(state));
    }
}


void
CharacterStateFalling::enter(CharacterBase& character)
{
    character.setupAnimation({
            .frameSize={80,96}
           ,.startFrame={3,2}
           ,.numFrames=4
           ,.duration = sf::seconds(1)
           ,.repeat = true
           ,.animationId = Textures::CharacterID::IchiFallingLow});
}

void
CharacterStateFalling::exit(CharacterBase& character)
{

}

//} /// namespace ryu