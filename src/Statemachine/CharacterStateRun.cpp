#include <Ryu/Statemachine/CharacterStateRun.h>
#include <Ryu/Statemachine/CharacterStateIdle.h>
#include <Ryu/Character/CharacterBase.h>
#include <Ryu/Character/CharacterIchi.h>


#include <iostream>
#include <memory>

//namespace ryu{

CharacterStateRun::CharacterStateRun(){}

CharacterStateRun::~CharacterStateRun(){}

std::unique_ptr<CharacterState> 
CharacterStateRun::handleInput(CharacterBase& character,EInput input)
{
    switch (input)
    {
       case EInput::ReleaseLeft:
       case EInput::ReleaseRight:
       case EInput::ReleaseUp:
       case EInput::ReleaseDown:
       {
           return std::move(std::make_unique<CharacterStateIdle>());    
       }
       break;
     
    default:
        break;
    }

    return nullptr;
}
     
void 
CharacterStateRun::update(CharacterBase& character)
{
    
    //character.move(character.getPlayerSpeed(),0.f);
    character.mIsMovingRight = true;
    /* move only mainchar ?
    if (auto ichi = dynamic_cast<CharacterIchi*>(character))
    {
        ichi.
    }
    */
   std::cout << "RUN UPDATE" << "\n";

}

void
CharacterStateRun::enter(CharacterBase& character)
{
    character.changeColor(sf::Color::Green);
    character.setTextureOnCharacter(Textures::CharacterID::IchiKatanaWalk);
}

void
CharacterStateRun::exit(CharacterBase& character)
{
    character.mIsMovingRight=false;
    //character.changeColor(sf::Color::Magenta);
}

//} /// namespace ryu