#pragma once

#include <SFML/Graphics.hpp>

enum class EMoveDirecton
{
	NONE,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	UPLEFT,
	UPRIGHT,
	DOWNLEFT,
	DOWNRIGHT
};

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
	sf::CircleShape mPlayer;
	EMoveDirecton mMoveDirection;
	bool mIsMovingUp=false;
	bool mIsMovingDown=false;
	bool mIsMovingLeft=false;
	bool mIsMovingRight=false;

	float PlayerSpeed = 75.f;
};
