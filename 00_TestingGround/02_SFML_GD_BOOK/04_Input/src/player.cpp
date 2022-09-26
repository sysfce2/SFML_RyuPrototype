#include "player.h"
#include "aircarft.h"
#include "category.h"

struct AircraftMover
{
	AircraftMover(float vx, float vy)
	: velocity(vx, vy)
	{
	}

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
    }
}

void
Player::setActions()
{
    Command moveLeft;
    float playerSpeed = 50.f;
    moveLeft.category = static_cast<unsigned>(Category::Type::Player);
    moveLeft.action = derivedAction<Aircraft>(AircraftMover(-playerSpeed, 0.f));
}

void
Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{}

void
Player::handleRealtimeInput(CommandQueue& commands)
{}