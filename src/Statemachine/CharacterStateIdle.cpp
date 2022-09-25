#include <Ryu/Character/CharacterBase.h>
#include <Ryu/Statemachine/CharacterStateIdle.h>
#include <Ryu/Statemachine/CharacterStateRun.h>
#include <Ryu/Statemachine/CharacterState.h>

#include <iostream>
#include <memory>
#include <type_traits>
#include <typeinfo>


using namespace ryu;

CharacterStateIdle::CharacterStateIdle(){}

CharacterStateIdle::~CharacterStateIdle(){}

std::unique_ptr<CharacterState>
CharacterStateIdle::handleInput(CharacterBase& character,EInput input)
{
    switch (input)
    {
       case EInput::PRESSLEFT:
       case EInput::PRESSRIGHT:
       {
           return std::move(std::make_unique<CharacterStateRun>());    
       }

       case EInput::PRESSUP:
       case EInput::PRESSDOWN:
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
CharacterStateIdle::update(CharacterBase& character)
{
    //std::cout << "IDLE"<< std::endl;
}


void
CharacterStateIdle::enter(CharacterBase& character)
{
    std::cout << "state idle enter " << std::endl;
    //std::cout << decltype(character) << '\n';
    std::cout << "chartype: " << typeid(character).name() << std::endl;
    character.setTextureOnCharacter(Textures::CharacterID::IchiIdleRun);
}

void
CharacterStateIdle::exit(CharacterBase& character)
{

}