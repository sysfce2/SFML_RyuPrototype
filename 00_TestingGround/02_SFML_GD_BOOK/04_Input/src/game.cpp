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
, mPlayerSpite()
, mWorld(mWindow)
, mStatisticsUpdateTime()
, mStatisticsNumFrames(0)
, mIsPaused(false)
{

	mPlayerSpite.setPosition(10.f,10.f);
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
			if(!mIsPaused) /// simple pause through gaied & lostfocus -events
			{
				update(TimePerFrame);
			}	
		}
		// show Fps etc / debug messages
		updateStatistics(elapsedTime);	
		render();
	}
}

void Game::processEvents()
{

	CommandQueue& commands = mWorld.getActiveCommands();

	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		switch(event.type)
		{
			mPlayer.handleEvent(event, commands);

			case sf::Event::GainedFocus:
			{
				mIsPaused = false;
				break;
			}
			case sf::Event::LostFocus:
			{
				mIsPaused = true;
				break;
			}
			case sf::Event::Closed:
			{
				mWindow.close();
				break;
			}
		}
	}

	mPlayer.handleRealtimeInput(commands);
}

void Game::update(sf::Time deltaTime)
{

	mWorld.update(deltaTime);
	
	
	//std::cout << std::to_string(movement.x) << "," << std::to_string(movement.y) << std::endl;
}

void Game::render()
{
	mWindow.clear();
	mWorld.draw();

	mWindow.setView(mWindow.getDefaultView());
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