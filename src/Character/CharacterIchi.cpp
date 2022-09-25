#include <Ryu/Character/CharacterIchi.h>

#include <iostream>
using namespace ryu;

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
    ichiTextureManager.load(ryu::Textures::CharacterID::IchiIdleRun,"assets/spritesheets/ichi/01_sheet_ichi_run.png");
}