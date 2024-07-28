#include <Ryu/Statemachine/CharacterStateLedgeClimbUp.h>
#include <Ryu/Statemachine/CharacterStateLedgeHang.h>


#include <Ryu/Character/CharacterBase.h>
#include <Ryu/Statemachine/CharacterStateIdle.h>
#include <Ryu/Statemachine/CharacterState.h>

#include <SFML/System/Vector2.hpp>
#include <memory>
#include <fmt/core.h>

//namespace ryu{

CharacterStateLedgeHang::CharacterStateLedgeHang(){}

CharacterStateLedgeHang::~CharacterStateLedgeHang(){}

std::unique_ptr<CharacterState>
CharacterStateLedgeHang::handleInput(CharacterBase& character,EInput input)
{
    switch (input)
    {
       case EInput::PressLeft:
       case EInput::PressRight:
       {
           break;
       }

       case EInput::PressUp:
       {
           return std::move(std::make_unique<CharacterStateLedgeClimbUp>());
       }

       case EInput::PressDown:
       {
           return std::move(std::make_unique<CharacterStateIdle>());
       }

    default:
        break;
    }

    return nullptr;
}

void
CharacterStateLedgeHang::update(CharacterBase& character)
{
}


void
CharacterStateLedgeHang::enter(CharacterBase& character)
{
    //TODO: temporary -> this should be automatically added !
    //auto& spriteAni = character.getSpriteAnimation();
    auto& spriteAni = character.getSpriteAnimation();
    auto pivot = spriteAni.getPivotAbs();
    auto posSprite = spriteAni.getPosition();
    auto origSprite = spriteAni.getOrigin();

    character.getBody()->SetGravityScale(0); // "flymode"
    character.setupAnimation(Textures::CharacterID::IchiLedgeHangIdle);
    character.setCharacterStateEnum(ECharacterState::Hanging);
    sf::Vector2f vec{(float)posSprite.x,(float)posSprite.y-82.f};
    spriteAni.setAnimationPosition(vec);
    // FIXME:-82 ? oh when setting here an offset the raycast will also be set shortly and this creates
    // a flickering bc sw it also will try to set the animation ...
    // so raycast should be set with the animation but with the physics body !!!
}

void
CharacterStateLedgeHang::exit(CharacterBase& character)
{
    character.getSpriteAnimation().restart();
    character.getBody()->SetGravityScale(4.8);
}

//} /// namespace ryu
