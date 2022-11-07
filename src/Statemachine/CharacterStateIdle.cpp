#include <Ryu/Character/CharacterBase.h>
#include <Ryu/Statemachine/CharacterStateIdle.h>
#include <Ryu/Statemachine/CharacterStateRun.h>
#include <Ryu/Statemachine/CharacterState.h>

#include <iostream>
#include <memory>
#include <type_traits>
#include <typeinfo>


//namespace ryu{

CharacterStateIdle::CharacterStateIdle(){}

CharacterStateIdle::~CharacterStateIdle(){}

std::unique_ptr<CharacterState>
CharacterStateIdle::handleInput(CharacterBase& character,EInput input)
{
    switch (input)
    {
       case EInput::PressLeft:
       case EInput::PressRight:
       {
           return std::move(std::make_unique<CharacterStateRun>());    
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
    
    // TODO: animation spec tospecial class / manager ?
    character.getSpriteAnimation().setFrameSize(sf::Vector2i(80,96));
    character.getSpriteAnimation().setStartFrame({0,0});
    character.getSpriteAnimation().setNumFrames(1);
    character.getSpriteAnimation().setDuration(sf::seconds(1));
    character.getSpriteAnimation().setRepeating(true);
    character.setTextureOnCharacter(Textures::CharacterID::IchiIdleRun);
    // set origin of texture to center
    sf::FloatRect bounds = character.getSpriteAnimation().getSprite().getLocalBounds();
    character.getSpriteAnimation().getSprite().setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    //mSprite.setOrigin(120.f, 144.f);
    character.updatePhysics();
    std::cout << "Boundswidth: " << bounds.width << "Boundsheight: " << bounds.height << "\n";
    //character.initPhysics(phWorld);
}

void
CharacterStateIdle::exit(CharacterBase& character)
{

}

//} /// namespace ryu