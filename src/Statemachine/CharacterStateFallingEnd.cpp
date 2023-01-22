#include <Ryu/Character/CharacterBase.h>
#include <Ryu/Statemachine/CharacterStateFallingEnd.h>
#include <Ryu/Statemachine/CharacterStateIdle.h>
#include <Ryu/Statemachine/CharacterState.h>
#include <Ryu/Core/Utilities.h>

#include <iostream>
#include <memory>


//namespace ryu{

CharacterStateFallingEnd::CharacterStateFallingEnd()
    :  touchedFloor(false)
     , timerTimeInMs(0.0f)
{
    std::cout << "FallingEnd-cdor\n";
}

CharacterStateFallingEnd::~CharacterStateFallingEnd(){
    std::cout << "FallingEnd-dtor\n";
}

std::unique_ptr<CharacterState>
CharacterStateFallingEnd::handleInput(CharacterBase& character,EInput input)
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
CharacterStateFallingEnd::update(CharacterBase& character)
{
    if(character.getSpriteAnimation().isFinished())
    {
        std::unique_ptr<CharacterStateIdle> state = std::make_unique<CharacterStateIdle>();
        character.changeState(std::move(state));
    }
}

void
CharacterStateFallingEnd::enter(CharacterBase& character)
{

        character.setupAnimation({
            .frameSize={80,96}
           ,.startFrame={0,4}
           ,.numFrames=11
           ,.duration = sf::milliseconds(1500)
           ,.repeat = false
           ,.animationId = Textures::CharacterID::IchiEndFallingLow});
}

void
CharacterStateFallingEnd::exit(CharacterBase& character)
{
    std::cout << "Exit: FallingEnd \n";
}

//} /// namespace ryu