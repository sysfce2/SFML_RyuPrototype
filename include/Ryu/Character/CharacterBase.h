#pragma once

#include <SFML/Graphics.hpp>
#include <Ryu/Statemachine/CharacterState.h>
#include <Ryu/Core/AssetManager.h>
#include <Ryu/Core/AssetIdentifiers.h>
#include <Ryu/Control/CharacterEnums.h>
#include <Ryu/Scene/SceneNode.h>

namespace sf{
    class Event;
}

class CommandQueue;

//namespace ryu{

class CharacterBase : public SceneNode {

    public:
        // TODO: implement rule of 5 !
        // (morph one character into another ^^)
        CharacterBase();
        explicit CharacterBase(ECharacterState startState);
        ~CharacterBase();
        float getPlayerSpeed() {return PlayerSpeed;}


        virtual void handleInput(EInput input);
        virtual void update(sf::Time deltaTime);
        virtual void loadTextures();
        
        virtual void setTextureOnCharacter(Textures::CharacterID textureId);
        virtual void setTexture(AssetManager<sf::Texture, Textures::CharacterID> &textureManager, Textures::CharacterID id);
        sf::Drawable& getSprite() { return mCharacterSprite;}

        void changeColor(sf::Color color);

        void setMovement(sf::Vector2f _movement);
        // TODO: make private/setter and make Vector or st ? -> see Game.cpp
        bool mIsMovingUp=false;
        bool mIsMovingDown=false;
        bool mIsMovingLeft=false;
        bool mIsMovingRight=false;

    protected:
        // TODO: use smart pointers instead ?!!! and move semantics
        //CharacterState* mCharacterState;
        std::unique_ptr<CharacterState> mCharacterState;
        ECharacterState mECharacterState;

    private:
        sf::Sprite mCharacterSprite;
        EMoveDirecton mMoveDirection;
        
        sf::Vector2f movement;
        float PlayerSpeed = 75.f;

};

//} /// namespace ryu