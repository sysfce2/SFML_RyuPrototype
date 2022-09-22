#include <Ryu/Character/CharacterIchi.h>

using namespace ryu;

CharacterIchi::CharacterIchi(ECharacterState startState)
: CharacterBase(startState)
{
    loadTextures();    
}

void
CharacterIchi::setTextureOnCharacter(Textures::CharacterID textureId)
{
    setTexture(ichiTextureManager, textureId);
}

void
CharacterIchi::loadTextures()
{
    ichiTextureManager.load(ryu::Textures::CharacterID::IchiIdleRun,"assets/spritesheets/ichi/01_sheet_ichi_run.png");
}