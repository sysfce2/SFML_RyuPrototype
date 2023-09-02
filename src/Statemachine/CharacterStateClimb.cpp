
#include <Ryu/Character/CharacterBase.h>
#include <Ryu/Statemachine/CharacterStateClimb.h>
#include <Ryu/Statemachine/CharacterState.h>

#include <iostream>
#include <memory>


//namespace ryu{

CharacterStateClimb::CharacterStateClimb(){}

CharacterStateClimb::~CharacterStateClimb(){}

std::unique_ptr<CharacterState>
CharacterStateClimb::handleInput(CharacterBase& character,EInput input)
{
    switch (input)
    {
       case EInput::PressLeft:
       case EInput::PressRight:
       {
           return nullptr;//std::move(std::make_unique<CharacterStateRun>());    
       }

       case EInput::PressUp:
       case EInput::PressDown:
       {
           // JUMP/DUCK
           //return new CharacterStateRun();    
       }     
    default:
        break;
    }

    return nullptr;
}
     
void 
CharacterStateClimb::update(CharacterBase& character)
{
    //std::cout << "IDLE"<< std::endl;
}


void
CharacterStateClimb::enter(CharacterBase& character)
{
    character.setupAnimation({
                .frameSize={80,96}
               ,.startFrame={0,0}
               ,.numFrames=1
               ,.duration = sf::seconds(1)
               ,.repeat = true
               ,.animationId = Textures::CharacterID::IchiIdle});
}

void
CharacterStateClimb::exit(CharacterBase& character)
{

}

//} /// namespace ryu
