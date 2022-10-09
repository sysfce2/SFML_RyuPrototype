#pragma once

#include <SFML/Graphics.hpp>
#include <Ryu/Animation/Animation.h>
#include <Ryu/Animation/SpritesheetAnimation.h>
#include <Ryu/Statemachine/CharacterState.h>
#include <Ryu/Core/AssetManager.h>
#include <Ryu/Core/AssetIdentifiers.h>
#include <Ryu/Control/CharacterEnums.h>
#include <Ryu/Scene/SceneNode.h>
#include <Ryu/Events/Subject.h>
#include <Ryu/Events/EventEnums.h>

namespace sf{
    class Event;
}

class CommandQueue;

//namespace ryu{

class CharacterBase : public SceneNode , public Subject
{

    public:
        // TODO: implement rule of 5 !
        // (morph one character into another ^^)
        CharacterBase();
        explicit CharacterBase(ECharacterState startState);
        ~CharacterBase();
        
        float getCharacterSpeed() {return mCharacterSpeed;}
        void setCharacterSpeed(float speed) {mCharacterSpeed = speed;};
        
        std::unique_ptr<CharacterState>& getCurrentCharacterState();

        virtual void setTextureOnCharacter(Textures::CharacterID textureId);
        virtual void setTexture(AssetManager<sf::Texture, Textures::CharacterID> &textureManager, Textures::CharacterID id);
        SpritesheetAnimation& getSpriteAnimation() { return mCharacterAnimation;}

        void setMovement(sf::Vector2f _movement);
        void setMoveDirection(EMoveDirecton _movementDir);
        EMoveDirecton getMoveDirection() {return mMoveDirection;}

        virtual void handleInput(EInput input);
        virtual void update(sf::Time deltaTime);
        virtual void loadTextures();
                
        void changeColor(sf::Color color);

        void notifyObservers(Event event);

    protected:
        SpritesheetAnimation mCharacterAnimation;
        
        std::unique_ptr<CharacterState> mCharacterState;
        ECharacterState mECharacterState;
        float mCharacterSpeed;
    
    private:
        EMoveDirecton mMoveDirection;
        sf::Vector2f movement;
};

//} /// namespace ryu