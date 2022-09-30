#include <Ryu/Character/CharacterIchi.h>

#include <iostream>
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
    std::cout << "set text. " << std::endl;
    setTexture(ichiTextureManager, textureId);
}

void
CharacterIchi::loadTextures()
{
    std::cout << "ichi loadtex " << std::endl;
    ichiTextureManager.load(Textures::CharacterID::IchiIdleRun,"assets/spritesheets/ichi/01_sheet_ichi_run.png");
}

void
CharacterIchi::moveCharacter(sf::Vector2f velocity)
{
    // do we need this ?
    //setVelocity(getVelocity()+ velocity);
}

//} /// namespace ryu