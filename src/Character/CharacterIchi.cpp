#include <Ryu/Character/CharacterIchi.h>

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
    std::cout << "set text. " << std::endl;
    setTexture(ichiTextureManager, textureId);
}

void
CharacterIchi::loadTextures()
{
    std::cout << "ichi loadtex " << std::endl;
    // Outfit normal
    ichiTextureManager.load(Textures::CharacterID::IchiIdleRun,"assets/spritesheets/ichi/01_sheet_ichi_run.png");
    // Outfit combat
    ichiTextureManager.load(Textures::CharacterID::IchiKatanaWalk,"assets/spritesheets/ichi/02_sheet_ichi_katana_walk.png");
}

void
CharacterIchi::moveCharacter(sf::Vector2f velocity)
{
    std::cout << "MOVE CHAR " /* std::string(velocity.x) <<*/ << "\n";
    //this->move(getVelocity()+ velocity);
    // do we need this ?
    //setVelocity(getVelocity()+ velocity);
}

//} /// namespace ryu