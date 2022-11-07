#pragma once

#include <Ryu/Animation/Animation.h>
#include <Ryu/Animation/SpritesheetAnimation.h>
#include <Ryu/Statemachine/CharacterState.h>
#include <Ryu/Core/AssetManager.h>
#include <Ryu/Core/AssetIdentifiers.h>
#include <Ryu/Control/CharacterEnums.h>
#include <Ryu/Scene/SceneNode.h>
#include <Ryu/Events/Subject.h>
#include <Ryu/Events/EventEnums.h>

#include <Thirdparty/glm/glm.hpp>
#include <SFML/Graphics.hpp>
#include <memory.h>

namespace sf{
    class Event;
}

class CommandQueue;
class b2World;
class b2Body;
class b2Fixture;

//namespace ryu{

class CharacterBase : public SceneNode , public Subject
{

    public:
        // TODO: implement rule of 5 !
        // (morph one character into another ^^)
        CharacterBase(std::unique_ptr<b2World>& phWorld,  const sf::Vector2f &position);
        CharacterBase(ECharacterState startState,  std::unique_ptr<b2World>& phWorld,  const sf::Vector2f &positiono);
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
        void updatePhysics();
        void updatePhysics(const sf::Vector2f &position);

        virtual void handleInput(EInput input);
        virtual void update(sf::Time deltaTime);
        void updateCharacterState(sf::Time deltaTime);
        virtual void loadTextures();
                
        void changeColor(sf::Color color);

        void notifyObservers(Event event);

        b2Body* getBody(){return mBody;}
        b2Fixture* getFixture(){return mFixture;}

    protected:
        /***
         * \brief   Initialized physic (body, fixtures for the character).
         *          its important to call the method in the child class, 
         *          because just there the animation sprite is set and the size & mass could be calculated 
         *  
        ***/
        void initPhysics(std::unique_ptr<b2World>& phWorld,  const sf::Vector2f &position);
        void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
 
        sf::Shape* getShapeFromCharPhysicsBody(b2Body* physicsBody) const;


        SpritesheetAnimation mCharacterAnimation;
        
        std::unique_ptr<CharacterState> mCharacterState;
        ECharacterState mECharacterState;
        float mCharacterSpeed;
    
    private:
        EMoveDirecton mMoveDirection;
        sf::Vector2f movement;

        // physics
        std::unique_ptr<b2World>& phWorldRef;
        b2Body* mBody;
        b2Fixture* mFixture;
};

//} /// namespace ryu