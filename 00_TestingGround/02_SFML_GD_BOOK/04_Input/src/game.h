#pragma once

#include <SFML/Graphics.hpp>
#include "world.h"
#include "player.h"


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
	void updateStatistics(sf::Time elapsedTime);

  private:
	sf::RenderWindow mWindow;
	//sf::Texture mTexture;
	sf::Sprite mPlayerSpite;
	Player mPlayer;
	World mWorld;
	//sf::Font mFont;
	sf::Text mFpsText;
	sf::Time mStatisticsUpdateTime;
	std::size_t mStatisticsNumFrames;

	EMoveDirecton mMoveDirection;
	bool mIsPaused;

	float PlayerSpeed = 75.f;
};
