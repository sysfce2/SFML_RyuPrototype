#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>
#include "assetmanager.h"
#include "game.h"

const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder2;
typedef ResourceHolder<sf::Font,std::string> FontHolder2;


Game::Game()
: mWindow(sf::VideoMode(1024, 768), "SFML Application")
, mPlayer()
, mStatisticsUpdateTime()
, mStatisticsNumFrames(0)
, mIsMovingUp(false)
, mIsMovingDown(false)
, mIsMovingRight(false)
, mIsMovingLeft(false)
{

	mPlayer.setPosition(10.f,10.f);
	// enable VSYNC: sf::RenderWindow::setVerticalSyncEnabled();

	mFpsText.setPosition(5.f,5.f);
	mFpsText.setCharacterSize(18);

	// check maxsize of textures
	//std::cout << mTexture.getMaximumSize() << " kb max for texture." << std::endl;
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	//TODO/FIXME: first load Textures here!
	// Be aware that Textures lifespan needs to be equal to the lifespan of the sprite using the texture
	// when putting this in the cdor the texture lives only within the constructor althought the game object
	// will live until the game dies
	// the path to load the textures from is /build/[TEXTPATH])
	TextureHolder2 textureManager;
	textureManager.load(Textures::ID::Eagle,"Media/Textures/Eagle.png");
	
	mPlayer.setTexture(textureManager.getResource(Textures::ID::Eagle));
	
	FontHolder2 fontManager;
	fontManager.load("sansation","Media/Fonts/Sansation.ttf");

	mFpsText.setFont(fontManager.getResource("sansation"));

	while (mWindow.isOpen())
	{
		processEvents();
		// use fixed time steps
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;
			processEvents();
			update(TimePerFrame);
		}
		// show Fps etc / debug messages
		updateStatistics(elapsedTime);	
		render();
	}
}

void Game::handleUserInput(sf::Keyboard::Key key, bool keyPressed)
{
	std::cout << std::to_string(key) << (keyPressed ? " pressed" : " released") << std::endl;
	switch(key)
	{
		case sf::Keyboard::W:
		case sf::Keyboard::Up:
		{
			mIsMovingUp = keyPressed;
			break;
		}

		case sf::Keyboard::S:
		case sf::Keyboard::Down:
		{
			mIsMovingDown = keyPressed;
			break;
		}
		case sf::Keyboard::D:
		case sf::Keyboard::Right:
		{
			mIsMovingRight = keyPressed;
			break;
		}
		case sf::Keyboard::A:
		case sf::Keyboard::Left:
		{
			mIsMovingLeft = keyPressed;
			break;
		}
	}
}

void Game::processEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		switch(event.type)
		{
			case sf::Event::KeyPressed:
			{
				handleUserInput(event.key.code,true);
				break;
			}
			case sf::Event::KeyReleased:
			{
				handleUserInput(event.key.code,false);
				break;
			}
			case sf::Event::Closed:
			{
				mWindow.close();
				break;
			}
		}
	}
}

void Game::update(sf::Time deltaTime)
{
	sf::Vector2f movement(0.f, 0.f);
	if(mIsMovingUp)
		movement.y -= PlayerSpeed;
	if(mIsMovingDown)
		movement.y += PlayerSpeed;
	if(mIsMovingLeft)
		movement.x -= PlayerSpeed;
	if(mIsMovingRight)
		movement.x += PlayerSpeed;

	mPlayer.move(movement * deltaTime.asSeconds());
	//std::cout << std::to_string(movement.x) << "," << std::to_string(movement.y) << std::endl;
}

void Game::render()
{
	mWindow.clear();
	mWindow.draw(mPlayer);
	mWindow.draw(mFpsText);
	mWindow.display();
}

void Game::updateStatistics(sf::Time elapsedTime)
{
	mStatisticsUpdateTime += elapsedTime;
	mStatisticsNumFrames += 1;

	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		mFpsText.setString(
			"FPS = " + std::to_string(mStatisticsNumFrames) + "\n" +
			"TickTime = " + std::to_string(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us");
							 
		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}