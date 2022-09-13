#pragma once

#include <SFML/Graphics.hpp>
#include <Ryu/Character/CharacterBase.h>


#include <memory>

namespace ryu{

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
	std::unique_ptr<CharacterBase> mPlayer;

};

} /// namespace ryu