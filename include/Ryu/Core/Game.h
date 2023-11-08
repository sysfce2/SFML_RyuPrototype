#pragma once

#include <SFML/Graphics.hpp>
#include <Ryu/Character/CharacterIchi.h>
#include <Ryu/Control/PlayerController.h>
#include <Ryu/Core/World.h>
#include <Ryu/Debug/imGuiDebug.h>
#include <Ryu/Events/Observer.h>
#include <RyuSuite/RAnimator.h>

#include <memory>

class CharacterIchi;
class PlayerController;
class CommandQueue;

//namespace ryu{

class Game : public Observer
{
  public:
	  Game();
	  void run();
    World& getWorld() {return mWorld;}
    void onNotify(const SceneNode& entity, RyuEvent event) override;
  private:
	  void processEvents(sf::Event& event, CommandQueue& commands);
	  void update(sf::Time deltaTime);
	  void render();
	  void handleUserInput(sf::Keyboard::Key key, bool keyPressed);
	  void addObservers();
    void setDebugValues();
  private:
	  sf::RenderWindow mWindow;
	  World mWorld;
    RyuAnimator::Editor mAnimator;
	  bool mIsPaused;
	  std::unique_ptr<PlayerController> mPlayerController;
    RyuDebug::DebugWidgets mDebugWidgets;
	//ryu::AssetManager<sf::Font,std::string> fontManager;
};

//} /// namespace ryu
