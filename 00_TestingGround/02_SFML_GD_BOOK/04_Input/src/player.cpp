#include "player.h"
#include "aircarft.h"
#include "category.h"

struct AircraftMover
{
	AircraftMover(float vx, float vy)
	: velocity(vx, vy)
	{
	}

	void operator() (SceneNode& node, sf::Time) const
	{
        // downcast SceneNode -> Aircrat needable
        Aircraft& aircraft = static_cast<Aircraft&>(node);
		aircraft.accelerate(velocity);
	}

	sf::Vector2f velocity;
};

void
Player::setActions()
{
    Command moveLeft;
    float playerSpeed = 50.f;
    moveLeft.category = static_cast<unsigned>(Category::Type::Player);
    moveLeft.action = AircraftMover(-playerSpeed, 0.f);
}

void
Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{}

void
Player::handleRealtimeInput(CommandQueue& commands)
{}