#include <SFML/Graphics.hpp>

#include <iostream>
#include <memory>

#include <Ryu/Core/Game.h>
#include <Ryu/Core/World.h>
#include <Ryu/Core/AssetManager.h>
#include <Ryu/Core/AssetIdentifiers.h>
#include <Ryu/Character/CharacterBase.h>

using namespace ryu;

const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
: mWindow(sf::VideoMode(1024, 768), "SFML Application")
,mPlayer(std::make_unique<CharacterIchi>(CharacterBase::ECharacterState::Idle))
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

void Game::handleUserInput(sf::Keyboard::Key key, bool keyPressed)
{
	//std::cout << std::to_string(key) << (keyPressed ? " pressed" : " released") << std::endl;
	switch(key)
	{
		case sf::Keyboard::W:
		case sf::Keyboard::Up:
		{
			mPlayer->mIsMovingUp = keyPressed;
			mPlayer->handleInput(keyPressed ? EInput::PRESSUP : EInput::RELEASEUP);
			break;
		}

		case sf::Keyboard::S:
		case sf::Keyboard::Down:
		{
			mPlayer->mIsMovingDown = keyPressed;
			mPlayer->handleInput(keyPressed ? EInput::PRESSDOWN : EInput::RELEASEDOWN);
			break;
		}
		case sf::Keyboard::D:
		case sf::Keyboard::Right:
		{
			mPlayer->mIsMovingRight = keyPressed;
			mPlayer->handleInput(keyPressed ? EInput::PRESSRIGHT : EInput::RELEASERIGHT);
			break;
		}
		case sf::Keyboard::A:
		case sf::Keyboard::Left:
		{
			mPlayer->mIsMovingLeft = keyPressed;
			mPlayer->handleInput(keyPressed ? EInput::PRESSLEFT : EInput::RELEASELEFT);
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
