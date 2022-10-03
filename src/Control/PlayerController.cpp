#include <iostream>
#include <map>
#include <SFML/Window/Keyboard.hpp>

#include <Ryu/Control/PlayerController.h>
#include <Ryu/Control/CharacterEnums.h>

//#include "aircarft.h"
#include <Ryu/Core/Category.h>
#include <Ryu/Core/Command.h>
#include <Ryu/Core/CommandQueue.h>
#include <Ryu/Character/CharacterIchi.h>

class CharacterIchi;

struct CharacterMover
{
	CharacterMover(float vx, float vy)
	: velocity(vx, vy)
	{
	}

    // execute CharacterMover
	void operator() (CharacterIchi& character, sf::Time) const
	{
        character.moveCharacter(velocity);
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
PlayerController::initializeBindings()
{
    float playerSpeed = 10.0f;

    mKeyBinding[sf::Keyboard::Left] = Action::MoveLeft;
    mKeyBinding[sf::Keyboard::A] = Action::MoveLeft;
    mKeyBinding[sf::Keyboard::Right] = Action::MoveRight;
    mKeyBinding[sf::Keyboard::D] = Action::MoveRight;
    mKeyBinding[sf::Keyboard::Up] = Action::MoveUp;
    mKeyBinding[sf::Keyboard::W] = Action::MoveUp;
    mKeyBinding[sf::Keyboard::Down] = Action::MoveDown;
    mKeyBinding[sf::Keyboard::S] = Action::MoveDown;

    
    mActionBinding[Action::MoveLeft].action = derivedAction<CharacterIchi>(
            CharacterMover(-playerSpeed,0.f));
    mActionBinding[Action::MoveRight].action = derivedAction<CharacterIchi>(
            CharacterMover(playerSpeed,0.f));
    mActionBinding[Action::MoveUp].action = derivedAction<CharacterIchi>(
            CharacterMover(0.f,-playerSpeed));
    mActionBinding[Action::MoveDown].action = derivedAction<CharacterIchi>(
            CharacterMover(0.f,playerSpeed));

    /*
    mActionBinding[Action::MoveLeft].action = derivedAction<CharacterIchi>(playerCharacter->handleInput(EInput::PRESSLEFT));
    mActionBinding[Action::MoveRight].action = ;
    mActionBinding[Action::MoveUp].action = ;
    mActionBinding[Action::MoveDown].action = ;
    */

    for(auto& actionBinding : mActionBinding)
    {
        actionBinding.second.category = static_cast<unsigned>(Category::Type::Player); 
    }
}

PlayerController::PlayerController(std::unique_ptr<CharacterIchi> const &character)
: playerCharacter(character)
{
    initializeBindings();
}

void
PlayerController::handleEvent(const sf::Event& event, CommandQueue& commands)
{

    /*
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
    */

   /* use a binding map from Action to functioncall*/

    std::cout << "PlayerHandleEvent " << event.type << std::endl;
    // first test for one-time events
    if(event.type == sf::Event::KeyPressed)
    {
        switch (event.key.code)
        {
            case sf::Keyboard::P:
            {
                std::cout << "P pressed " << std::endl;
                Command output;
                output.category = static_cast<unsigned>(Category::Type::Player);
                output.action = [](SceneNode& s,sf::Time)
                {
                    std::cout << s.getPosition().x << "," << s.getPosition().y << "\n";
                };
                commands.push(output);
                break;
            }

            case sf::Keyboard::D:
            case sf::Keyboard::Right:
            {
                std::cout << "Right pressed " << std::endl;
                playerCharacter->handleInput(EInput::PRESSRIGHT);
                break;
            }

            case sf::Keyboard::A:
            case sf::Keyboard::Left:
            {
                std::cout << "Left pressed " << std::endl;
                playerCharacter->handleInput(EInput::PRESSLEFT);
                break;
            }
            default:
                break;
        }
    }
    if(event.type == sf::Event::KeyReleased)
    {
        switch (event.key.code)
        {
            case sf::Keyboard::D:
            case sf::Keyboard::Right:
            {
                std::cout << "Right released " << std::endl;
                playerCharacter->handleInput(EInput::RELEASERIGHT);
                break;
            }
            case sf::Keyboard::A:
            case sf::Keyboard::Left:
            {
                std::cout << "Left released " << std::endl;
                playerCharacter->handleInput(EInput::RELEASELEFT);
                break;
            }
            default:
                break;
        }
    }
}

void
PlayerController::handleRealtimeInput(CommandQueue& commands)
{
    for(auto const& binding : mKeyBinding)
    {
        if(sf::Keyboard::isKeyPressed(binding.first) && isRealtimeAction(binding.second))
        {
            commands.push(mActionBinding[binding.second]);
        }
    }

    /*
 	switch(key)
		case :
		case sf::Keyboard::Up:
		{
			mPlayer->handleInput(keyPressed ? EInput::PRESSUP : EInput::RELEASEUP);
		}

		case sf::Keyboard::S:
		case sf::Keyboard::Down:
		{
			mPlayer->handleInput(keyPressed ? EInput::PRESSDOWN : EInput::RELEASEDOWN);
		}
		case sf::Keyboard::D:
		case sf::Keyboard::Right:
		{
			mPlayer->handleInput(keyPressed ? EInput::PRESSRIGHT : EInput::RELEASERIGHT);
		}
		case sf::Keyboard::A:
		case sf::Keyboard::Left:
		{
			mPlayer->handleInput(keyPressed ? EInput::PRESSLEFT : EInput::RELEASELEFT);
		}
    */    
}

bool
PlayerController::isRealtimeAction(Action action)
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
PlayerController::assignKey(Action action, sf::Keyboard::Key key)
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
PlayerController::getAssignedKey(Action action) const
{
    for(auto& binding : mKeyBinding)
    {
        if (binding.second == action)
			return binding.first;
    }
	
	return sf::Keyboard::Unknown;
}