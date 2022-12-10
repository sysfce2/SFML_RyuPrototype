#include <SFML/Graphics.hpp>

#include <iostream>
#include <memory>

#include <Ryu/Core/Game.h>
#include <Ryu/Core/World.h>
#include <Ryu/Core/AssetManager.h>
#include <Ryu/Core/AssetIdentifiers.h>
#include <Ryu/Character/CharacterBase.h>
#include <Ryu/Character/CharacterIchi.h>
#include <Ryu/Control/PlayerController.h>
#include <Ryu/Control/CharacterEnums.h>


//namespace ryu{

const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
: mWindow(sf::VideoMode(1200, 800), "SFML Application")
,mWorld(mWindow)
,mPlayerController(std::make_unique<PlayerController>(mWorld.getPlayer()))
,mIsPaused(false)
{
	// todo: how to load ichis tzextures at  startup ? 
	//mPlayer->loadTextures();
	addObservers();
}

void
Game::addObservers()
{
	auto player = mWorld.getPlayer();
	player->addObserver(mPlayerController.get());
}

void Game::run()
{
	sf::Clock clock;
	// uses fixed tick steps (use same delta every time)
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	
	// TODO: why no debugdrawing ?????
	//mWorld.setDebugDrawer(mWindow);

	while (mWindow.isOpen())
	{
		processEvents();
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;
			processEvents();
			if (!mIsPaused)
			{
				update(TimePerFrame);
			}
		}	
		render();
	}
}

void Game::processEvents()
{
	CommandQueue& commands = mWorld.getActiveCommands();

	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		// Player-related one-time events
		mPlayerController->handleEvent(event, commands);

		switch(event.type)
		{		
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

	mPlayerController->handleRealtimeInput(commands);
}

void Game::update(sf::Time deltaTime)
{
	mWorld.update(deltaTime);
}

void Game::render()
{
	mWindow.clear();
	mWorld.draw();

	mWindow.setView(mWindow.getDefaultView());
	//TODO: statistics fps text ....
	mWindow.display();
}
//} /// namespace ryu