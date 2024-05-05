#pragma once

#include <Ryu/Character/CharacterBase.h>
#include <Ryu/Core/AssetIdentifiers.h>

#include <box2d/b2_math.h>

#include <Ryu/Physics/Raycast.h>
//namespace ryu {

using IchiTextureManager = AssetManager<sf::Texture, Textures::SpritesheetID>;


class CharacterIchi : public CharacterBase
{
    public:
        CharacterIchi(ECharacterState startState, std::unique_ptr<b2World>& phWorld,  const sf::Vector2f &position);
        void setTextureOnCharacter(Textures::SpritesheetID textureId) override;
    
        //TODO: make it private and load otherwise ? / atm. public for Game when creating the mPlayer in Cdor
        void loadTextures();
        void moveCharacter(sf::Vector2f velocity);
        unsigned int getCategory() const override;
        void update(sf::Time deltaTime) override;
        void onNotify(const SceneNode& entity, Ryu::EEvent event) override;
        bool getHit(std::string rcName) override;
        void eraseRaycast(std::string rcName) override;
        void teleportCharacter(float x, float y);
private:
        void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
        //void createRaycast(std::string type,std::pair<double,double> startPoint, float angle, float length);
        void allowedMovement(bool& xMove, bool& yMove);
        void checkContact(std::string name);
        IchiTextureManager ichiTextureManager;
        std::map<std::string, RayCastClosest> rayCastCallbacks;
};

//} /// namespace ryu
