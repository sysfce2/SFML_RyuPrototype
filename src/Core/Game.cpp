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
: mWindow(sf::VideoMode(1024, 768), "SFML Application")
,mPlayer(std::make_unique<CharacterIchi>(ECharacterState::Idle))
,mPlayerController(std::make_unique<PlayerController>(mPlayer))
,mWorld(mWindow)
,mIsPaused(false)
{
	// todo: how to load ichis tzextures at  startup ? 
	//mPlayer->loadTextures();
}

void Game::run()
{
	sf::Clock clock;
	// uses fixed tick steps (use same delta every time)
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	
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
	mPlayer->update(deltaTime);
	mWorld.update(deltaTime);
}

void Game::render()
{
	mWindow.clear();
	mWorld.draw();

	mWindow.setView(mWindow.getDefaultView());
	mWindow.draw(mPlayer->getSprite());
	//TODO: statistics fps text ....
	mWindow.display();
}
//} /// namespace ryu