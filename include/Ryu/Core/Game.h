#pragma once

#include <SFML/Graphics.hpp>
#include <Ryu/Character/CharacterIchi.h>
#include <Ryu/Control/PlayerController.h>
#include <Ryu/Core/World.h>

#include <memory>

class CharacterIchi;
class PlayerController;

//namespace ryu{

class Game
{
  public:
	Game();
	void run();
  private:
	void processEvents();
	void update(sf::Time deltaTime);
	void render();
	void handleUserInput(sf::Keyboard::Key key, bool keyPressed);
  private:
	sf::RenderWindow mWindow;
	World mWorld;
	bool mIsPaused;
	std::unique_ptr<PlayerController> mPlayerController;
	//ryu::AssetManager<sf::Font,std::string> fontManager;
};

//} /// namespace ryu