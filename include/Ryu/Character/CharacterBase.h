#pragma once

// #include <Ryu/Animation/Animation.h>
#include "Ryu/Animation/AnimationManager.h"
#include <Ryu/Animation/SpritesheetAnimation.h>
#include <Ryu/Control/CharacterEnums.h>
#include <Ryu/Core/AssetIdentifiers.h>
#include <Ryu/Core/AssetManager.h>
#include <Ryu/Events/EventEnums.h>
#include <Ryu/Events/Subject.h>
#include <Ryu/Events/Observer.h>
#include <Ryu/Scene/SceneNode.h>
#include <Ryu/Scene/EntityStatic.h>
#include <Ryu/Statemachine/CharacterState.h>

#include <SFML/Graphics.hpp>
#include <fmt/core.h>
#include <Thirdparty/glm/glm.hpp>
#include <box2d/b2_math.h>
#include <box2d/box2d.h>
#include <iostream>
#include <memory.h>

using BaseTextureManager = AssetManager<sf::Texture, Textures::PhysicAssetsID>;

namespace sf {
class Event;
}

class CommandQueue;
class b2World;
class b2Body;
class b2Vec2;
class b2Fixture;
// namespace ryu{

struct CharacterSetting {
  // due increase of gravityscale (falling is then more gamey) the physicbody
  // needs some adustments for movement so its not behind the movement of
  // the characteranimation
  float MoveMultiplierX = 1.05f;
  float MoveMultiplierY = 1.47f;

  b2Vec2 jumpForwardImpulse{150, -250};
  b2Vec2 jumpUpImpulse{0, -200};
  b2Vec2 massCenter{0, 0};
  float bodyMass{25};
};

struct CharacterFinalSetting {
  // due increase of gravityscale (falling is then more gamey) the physicbody
  // needs some adustments for movement so its not behind the movement of
  // the characteranimation
  float MoveMultiplierX;
  float MoveMultiplierY;

  b2Vec2 jumpForwardImpulse;
  b2Vec2 jumpUpImpulse;
  b2Vec2 massCenter;
  float bodyMass;

  CharacterFinalSetting()
  {
    MoveMultiplierX = 1.05f;
    MoveMultiplierY = 1.47f;

    jumpForwardImpulse = {150, -250};
    jumpUpImpulse = {0, -200};
    massCenter= {0, 0};
    bodyMass=25;

  }
};

struct AnimationConfiguration {
  sf::Vector2i frameSize;
  sf::Vector2i startFrame;
  std::size_t numFrames;
  sf::Time duration;
  bool repeat;
  Textures::CharacterID animationId;
};

static std::map<Textures::CharacterID, Textures::SpritesheetID> AnimationToSpritesheetID =
{
{Textures::CharacterID::IchiDuckEnter, Textures::SpritesheetID::Ichi80x96},
{Textures::CharacterID::IchiDuckIdle, Textures::SpritesheetID::Ichi80x96},
{Textures::CharacterID::IchiDuckWalk, Textures::SpritesheetID::Ichi80x96},
{Textures::CharacterID::IchiFallingLoop, Textures::SpritesheetID::Ichi80x96},
{Textures::CharacterID::IchiIdle, Textures::SpritesheetID::Ichi80x96},
{Textures::CharacterID::IchiJumpForwardNrml, Textures::SpritesheetID::Ichi80x96},
{Textures::CharacterID::IchiJumpUp, Textures::SpritesheetID::Ichi80x96},
{Textures::CharacterID::IchiLadderBack, Textures::SpritesheetID::Ichi80x96},
{Textures::CharacterID::IchiLadderClimbLoop, Textures::SpritesheetID::Ichi80x96},
{Textures::CharacterID::IchiLadderEnter, Textures::SpritesheetID::Ichi80x96},
{Textures::CharacterID::IchiLandHigh, Textures::SpritesheetID::Ichi80x96},
{Textures::CharacterID::IchiLadderIdle, Textures::SpritesheetID::Ichi80x96},
{Textures::CharacterID::IchiLandLow, Textures::SpritesheetID::Ichi80x96},
{Textures::CharacterID::IchiRun, Textures::SpritesheetID::Ichi80x96},
{Textures::CharacterID::IchiWalk, Textures::SpritesheetID::Ichi80x96},
{Textures::CharacterID::IchiStartFalling, Textures::SpritesheetID::Ichi80x96},
{Textures::CharacterID::IchiSword1Idle, Textures::SpritesheetID::Ichi80x96},
{Textures::CharacterID::IchiSword1Walk, Textures::SpritesheetID::Ichi80x96},
{Textures::CharacterID::IchiLedgeClimbUp, Textures::SpritesheetID::Ichi128x196},
{Textures::CharacterID::IchiLedgeHangIdle, Textures::SpritesheetID::Ichi128x196},
};

class AnimationManager;

class RyuContactListener : public b2ContactListener
{

  public:
    void BeginContact(b2Contact* contact)
    {
      auto userData = contact->GetFixtureA()->GetBody()->GetUserData();
      EntityStatic* entity = reinterpret_cast<EntityStatic*>(userData.pointer);
      fmt::print("Get Contact with {}, climbable: {}\n", userData.pointer, entity->getEntityType() == EntityType::Climbable ? "Yes" : "No" );
    }


};

class CharacterBase : public SceneNode, public Subject, public Observer {

public:
    // TODO: implement rule of 5 !
  // (morph one character into another ^^)
  CharacterBase(std::unique_ptr<b2World> &phWorld,
                const sf::Vector2f &position);
  CharacterBase(ECharacterState startState, std::unique_ptr<b2World> &phWorld,
                const sf::Vector2f &positiono);
  ~CharacterBase();

  float getCharacterSpeed() { return mCharacterSpeed; }
  void setCharacterSpeed(float speed) { mCharacterSpeed = speed; };

  void setupAnimation(Textures::CharacterID aniId);

  std::unique_ptr<CharacterState> &getCurrentCharacterState();

  virtual void setTextureOnCharacter(Textures::SpritesheetID textureId);
  virtual void
  setTexture(AssetManager<sf::Texture, Textures::SpritesheetID> &textureManager,
             Textures::SpritesheetID id);
  SpritesheetAnimation &getSpriteAnimation() { return mCharacterAnimation; }

  void setMovement(sf::Vector2f _movement);
  void setMoveDirection(EMoveDirection _movementDir);
  EMoveDirection getMoveDirection() { return mMoveDirection; }
  void initPhysics();
  void destroyPhysics();
  void updatePhysics();
  void updatePhysics(const sf::Vector2f &position);
  void checkClimbingState();

  virtual void handleInput(EInput input);
    virtual void update(sf::Time deltaTime);
    virtual bool getHit(std::string rcName) = 0;
    virtual void eraseRaycast(std::string rcName);
  void updateCharacterPosition(sf::Time deltaTime);
  virtual void loadTextures();
  void changeState(std::unique_ptr<CharacterState> toState);
  // void setupAnimation(AnimationConfiguration config);

  std::unique_ptr<b2World> &getPhysicsWorldRef() { return phWorldRef; }
  void changeColor(sf::Color color);

  void notifyObservers(Ryu::EEvent event);
  bool isFalling() { return mCharacterFalling; }

  b2Body *getBody() { return mBody; }
  b2Fixture *getFixture() { return mFixture; }
  ECharacterState getCharacterStateEnum() { return mECharacterState; }
  EActionHeight getActionHeight() { return mActionHeight; }
  void setCharacterStateEnum(ECharacterState stateValue) {
    mECharacterState = stateValue;
  }
  void setActionHeight(EActionHeight heightValue) {
    mActionHeight = heightValue;
  }
  b2Vec2 getLinearVelocity() { return mBody->GetLinearVelocity(); }
  bool allowedToFall();
  bool inDuckMode();
  void jumpUp();
  void jumpForward();
  float getDirectionMultiplier();
  void setCharacterSettings(CharacterSetting settings);
    void resetCharacterSettings();
    bool duckStateActive() {return mDuckStateActive;};
    void setDuckState(bool duckstate) {mDuckStateActive = duckstate;};
  virtual void onNotify(const SceneNode& entity, Ryu::EEvent event) override;
    void ouputAnimations() {mAnimationManager->outputStoredAnimations();}
    void toggleTestStates();
    void setPositionOffset(sf::Vector2f offset);
    void setOffset(bool state) {mSetOffset = state;};
    bool getOffsetState() { return mSetOffset;};
    sf::Vector2f getPositionCross() { return positionCrossOffset;}
protected:
  /***
   * \brief   Initialized physic (body, fixtures for the character).
   *          its important to call the method in the child class,
   *          because just there the animation sprite is set and the size & mass
   *could be calculated
   *
   ***/
  void initPhysics(std::unique_ptr<b2World> &phWorld,
                   const sf::Vector2f &position);
  void drawCurrent(sf::RenderTarget &target,
                   sf::RenderStates states) const override;

  sf::Shape *getShapeFromCharPhysicsBody(b2Body *physicsBody) const;

  SpritesheetAnimation mCharacterAnimation;

  std::unique_ptr<CharacterState> mCharacterState;
  ECharacterState mECharacterState;
  float mCharacterSpeed;
  bool physicsInitialized;



private:
    BaseTextureManager baseTextureManager;
    bool mDuckStateActive;
    int testStatesCount = 2;
    int testStateCurrent = 0;
    sf::Vector2f positionCrossOffset{0,0};
    sf::Vector2f lastPositionCrossOffset{0,0};
    bool mSetOffset;
    b2Vec2 mLastBodyPosition;
    RyuContactListener contactListener;

protected:
  std::unique_ptr<AnimationManager> mAnimationManager;
  EMoveDirection mMoveDirection;
  EActionHeight mActionHeight;

  sf::Vector2f movement;
  bool mCharacterFalling;

  // physics
  std::unique_ptr<b2World> &phWorldRef;
  b2Body *mBody;
  b2Fixture *mFixture;
  Textures::LevelID mCurrentLevel;

  ECharacterMovement mECharacterMovement;
public:
  std::map<std::string, std::pair<b2Vec2, b2Vec2>> rayCastPoints;
    CharacterSetting mCharSettings{};
    CharacterFinalSetting mFinalCharSettings{};

};

//} /// namespace ryu
