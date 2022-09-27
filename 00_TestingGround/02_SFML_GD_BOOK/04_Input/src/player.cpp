#include <iostream>
#include <map>
#include <SFML/Window.hpp>

#include "player.h"
#include "aircarft.h"
#include "category.h"
#include "command.h"
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
Player::initializeBindings()
{
    float playerSpeed = 10.0f;

    mKeyBinding[sf::Keyboard::Left] = Action::MoveLeft;
    mKeyBinding[sf::Keyboard::Right] = Action::MoveRight;
    mKeyBinding[sf::Keyboard::Up] = Action::MoveUp;
    mKeyBinding[sf::Keyboard::Down] = Action::MoveDown;

    mActionBinding[Action::MoveLeft].action = derivedAction<Aircraft>(
            AircraftMover(-playerSpeed,0.f));
    mActionBinding[Action::MoveRight].action = derivedAction<Aircraft>(
            AircraftMover(playerSpeed,0.f));
    mActionBinding[Action::MoveUp].action = derivedAction<Aircraft>(
            AircraftMover(0.f,-playerSpeed));
    mActionBinding[Action::MoveDown].action = derivedAction<Aircraft>(
            AircraftMover(0.f,playerSpeed));
    
    for(auto& actionBinding : mActionBinding)
    {
        actionBinding.second.category = static_cast<unsigned>(Category::Type::Player); 
    }
}

Player::Player()
{
    initializeBindings();
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
    for(auto const& binding : mKeyBinding)
    {
        if(sf::Keyboard::isKeyPressed(binding.first) && isRealtimeAction(binding.second))
        {
            commands.push(mActionBinding[binding.second]);
        }
    }    
}

bool
Player::isRealtimeAction(Action action)
{
	switch (action)
	{
		case Action::MoveLeft:
		case Action::MoveRight:
		case Action::MoveDown:
		case Action::MoveUp:
			return true;

		default:
			return false;
	}
}

void
Player::assignKey(Action action, sf::Keyboard::Key key)
{
	// Remove all keys that already map to action
	for (auto itr = mKeyBinding.begin(); itr != mKeyBinding.end(); )
	{
		if (itr->second == action)
			mKeyBinding.erase(itr++);
		else
			++itr;
	}

	// Insert new binding
	mKeyBinding[key] = action;
}

sf::Keyboard::Key
Player::getAssignedKey(Action action) const
{
    for(auto& binding : mKeyBinding)
    {
        if (binding.second == action)
			return binding.first;
    }
	
	return sf::Keyboard::Unknown;
}