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
    std::cout << "state falling enter " << std::endl;
    //std::cout << decltype(character) << '\n';
    std::cout << "chartype: " << typeid(character).name() << std::endl;
    
    // TODO: animation spec tospecial class / manager ?
    // at least in method in character with struct "animationConfig"
    character.getSpriteAnimation().setFrameSize(sf::Vector2i(80,96));
    character.getSpriteAnimation().setStartFrame({0,4});
    character.getSpriteAnimation().setNumFrames(5);
    character.getSpriteAnimation().setDuration(sf::seconds(1));
    character.getSpriteAnimation().setRepeating(true);
    character.setTextureOnCharacter(Textures::CharacterID::IchiFallingLow);
    // set origin of texture to center
    sf::FloatRect bounds = character.getSpriteAnimation().getSprite().getLocalBounds();
    character.getSpriteAnimation().getSprite().setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    //mSprite.setOrigin(120.f, 144.f);
    std::cout << "Boundswidth: " << bounds.width << "Boundsheight: " << bounds.height << "\n";
    character.initPhysics();

}

void
CharacterStateFalling::exit(CharacterBase& character)
{

}

//} /// namespace ryu