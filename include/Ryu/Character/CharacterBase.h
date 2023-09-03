#pragma once

// #include <Ryu/Animation/Animation.h>
#include <Ryu/Animation/SpritesheetAnimation.h>
#include "Ryu/Animation/AnimationManager.h"
#include <Ryu/Statemachine/CharacterState.h>
#include <Ryu/Core/AssetManager.h>
#include <Ryu/Core/AssetIdentifiers.h>
#include <Ryu/Control/CharacterEnums.h>
#include <Ryu/Scene/SceneNode.h>
#include <Ryu/Events/Subject.h>
#include <Ryu/Events/EventEnums.h>
#include <Ryu/Core/AssetIdentifiers.h>

#include <Thirdparty/glm/glm.hpp>
#include <SFML/Graphics.hpp>
#include <box2d/b2_math.h>
#include <box2d/box2d.h>
#include <memory.h>
#include <iostream>

using BaseTextureManager = AssetManager<sf::Texture, Textures::PhysicAssetsID>;


namespace sf{
    class Event;
}

class CommandQueue;
class b2World;
class b2Body;
class b2Vec2;
class b2Fixture;

//namespace ryu{

struct CharacterSetting
{
    // due increase of gravityscale (falling is then more gamey) the physicbody
    // needs some adustments for movement so its not behind the movement of
    // the characteranimation
    float MoveMultiplierX = 1.05f;
    float MoveMultiplierY = 1.47f;
    b2Vec2 JumpUpForce{0,50000};
};

struct AnimationConfiguration
{
    sf::Vector2i frameSize;
    sf::Vector2i startFrame;
    std::size_t numFrames;
    sf::Time duration;
    bool repeat;
    Textures::CharacterID animationId;
};

class AnimationManager;

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

        void setupAnimation(Textures::CharacterID aniId);

        std::unique_ptr<CharacterState>& getCurrentCharacterState();

        virtual void setTextureOnCharacter(Textures::LevelID textureId);
        virtual void setTexture(AssetManager<sf::Texture, Textures::LevelID> &textureManager, Textures::LevelID id);
        SpritesheetAnimation& getSpriteAnimation() { return mCharacterAnimation;}

        void setMovement(sf::Vector2f _movement);
        void setMoveDirection(EMoveDirection _movementDir);
        EMoveDirection getMoveDirection() {return mMoveDirection;}
        void initPhysics();
        void destroyPhysics();
        void updatePhysics();
        void updatePhysics(const sf::Vector2f &position);

        virtual void handleInput(EInput input);
        virtual void update(sf::Time deltaTime);
        void updateCharacterState(sf::Time deltaTime);
        virtual void loadTextures();
        void changeState(std::unique_ptr<CharacterState> toState);
        void setupAnimation(AnimationConfiguration config);

        std::unique_ptr<b2World>& getPhysicsWorldRef() {return phWorldRef;}
        void changeColor(sf::Color color);

        void notifyObservers(Ryu::EEvent event);
        bool isFalling() {return mCharacterFalling;}

        b2Body* getBody(){return mBody;}
        b2Fixture* getFixture(){return mFixture;}
        ECharacterState getCharacterStateEnum() {return mECharacterState;}
        EActionHeight getActionHeight() {return mActionHeight;}
        void setCharacterStateEnum(ECharacterState stateValue){mECharacterState = stateValue;}
        void setActionHeight(EActionHeight heightValue){mActionHeight = heightValue;}
        b2Vec2 getLinearVelocity() {return mBody->GetLinearVelocity(); }
        bool allowedToFall();
        void jumpUp();
    
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
        bool physicsInitialized;

    private:
        BaseTextureManager baseTextureManager;

    protected:
        std::unique_ptr<AnimationManager> mAnimationManager;
        EMoveDirection mMoveDirection;
        EActionHeight mActionHeight;
    
        sf::Vector2f movement;
        bool mCharacterFalling;

        // physics
        std::unique_ptr<b2World>& phWorldRef;
        b2Body* mBody;
        b2Fixture* mFixture;
        Textures::LevelID mCurrentLevel;

        ECharacterMovement mECharacterMovement;

    public:
        std::map<std::string, std::pair<b2Vec2,b2Vec2> > rayCastPoints;
        CharacterSetting mCharSettings;
};

//} /// namespace ryu
