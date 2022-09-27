#include <iostream>

#include "player.h"
#include "aircarft.h"
#include "category.h"
#include "commandqueue.h"

struct AircraftMover
{
	AircraftMover(float vx, float vy)
	: velocity(vx, vy)
	{
	}

    // execute AircraftMover
	void operator() (Aircraft& aircraft, sf::Time) const
	{
       aircraft.accelerate(velocity);
	}

	sf::Vector2f velocity;
};

// small adapter that takes a function on a derived class and converts it to a function on the SceneNode base class
template <typename GameObject, typename Function>
std::function<void(SceneNode&, sf::Time)> derivedAction(Function fn)
{
    return [=](SceneNode& node, sf::Time dt)
    {
        // check if cast is safe (debug-mode)
        assert(dynamic_cast<GameObject*>(&node) != nullptr);

        // downcast node and invoke function on it
        fn(static_cast<GameObject&>(node),dt);
    };
}

void
Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
    //std::cout << "PlayerHandleEvent" << std::endl;
    // first test for one-time events
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
    {
        Command output;
        output.category = static_cast<unsigned>(Category::Type::Player);
        output.action = [](SceneNode& s,sf::Time)
        {
            std::cout << s.getPosition().x << "," << s.getPosition().y << "\n";
        };
        commands.push(output);
    }
}

void
Player::handleRealtimeInput(CommandQueue& commands)
{
 
    const float playerSpeed = 10.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        //std::cout << "PlayerRealTimeinput (l)" << std::endl;

        Command moveLeft;
        moveLeft.category = static_cast<unsigned>(Category::Type::Player);
        moveLeft.action = derivedAction<Aircraft>(
            AircraftMover(-playerSpeed,0.f));
        commands.push(moveLeft);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        const float playerSpeed = 10.f;
        //std::cout << "PlayerRealTimeinput (r)" << std::endl;

        Command moveRight;
        moveRight.category = static_cast<unsigned>(Category::Type::Player);
        moveRight.action = derivedAction<Aircraft>(
            AircraftMover(playerSpeed,0.f));
        commands.push(moveRight);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        const float playerSpeed = 10.f;
        //std::cout << "PlayerRealTimeinput (u)" << std::endl;

        Command moveUp;
        moveUp.category = static_cast<unsigned>(Category::Type::Player);
        moveUp.action = derivedAction<Aircraft>(
            AircraftMover(0.f,-playerSpeed));
        commands.push(moveUp);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        const float playerSpeed = 10.f;
        //std::cout << "PlayerRealTimeinput (d)" << std::endl;

        Command moveDown;
        moveDown.category = static_cast<unsigned>(Category::Type::Player);
        moveDown.action = derivedAction<Aircraft>(
            AircraftMover(0.f,playerSpeed));
        commands.push(moveDown);
    }
}