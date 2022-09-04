#include <SFML/Graphics.hpp>

#include <iostream>
#include "game.h"

Game::Game()
: mWindow(sf::VideoMode(1024, 768), "SFML Application")
, mPlayer()
{
	mPlayer.setRadius(40.f);
	mPlayer.setPosition(100.f, 100.f);
	mPlayer.setFillColor(sf::Color::Cyan);
}

void Game::run()
{
	sf::Clock clock;
	while (mWindow.isOpen())
	{
		sf::Time deltaTime = clock.restart();
		processEvents();
		update(deltaTime);
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
	mWindow.display();
}
