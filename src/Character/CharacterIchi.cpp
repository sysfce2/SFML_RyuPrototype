#include <Ryu/Character/CharacterIchi.h>
#include <Ryu/Core/Category.h>

#include <iostream>
#include <string>
//namespace ryu {

CharacterIchi::CharacterIchi(ECharacterState startState)
: CharacterBase(startState)
 , ichiTextureManager()
{
    loadTextures();

    mCharacterState->enter(*this);    
}

void
CharacterIchi::setTextureOnCharacter(Textures::CharacterID textureId)
{
    setTexture(ichiTextureManager, textureId);
}

void
CharacterIchi::loadTextures()
{
    // Outfit normal
    ichiTextureManager.load(Textures::CharacterID::IchiIdleRun,"assets/spritesheets/ichi/01_sheet_ichi_run.png");
    // Outfit combat
    ichiTextureManager.load(Textures::CharacterID::IchiKatanaWalk,"assets/spritesheets/ichi/02_sheet_ichi_katana_walk.png");
}

unsigned int 
CharacterIchi::getCategory() const
{
    return static_cast<unsigned>(Category::Type::Player);
}

void
CharacterIchi::moveCharacter(sf::Vector2f velocity)
{
    std::cout << "MOVE CHAR " << velocity.x << "," << velocity.y << "\n";
    /* TODO: tmp here/constran movement*/

    bool xMove = true;
    bool yMove = true;

    if(velocity.x < 0)
    {
        setMoveDirection(EMoveDirecton::Left);
    }

    if(velocity.x > 0)
    {
        setMoveDirection(EMoveDirecton::Right);
    }

    if(velocity.y < 0)
    {
        setMoveDirection(EMoveDirecton::Up);
    }

    if(velocity.y > 0)
    {
        setMoveDirection(EMoveDirecton::Down);
    }


    if(velocity.x == 0.f && velocity.y == 0.f)
    {
        setMovement({0.f,0.f});
        setMoveDirection(EMoveDirecton::None);
    }

    // TODO: here we need to check if movement in the wanted direction is allowed
    if((xMove && velocity.x != 0.f) || (yMove && velocity.y != 0.f))
    {
        setMovement(velocity);
    }
}

//} /// namespace ryu