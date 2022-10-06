#include <iostream>
#include <map>
#include <SFML/Window/Keyboard.hpp>

#include <Ryu/Control/PlayerController.h>
#include <Ryu/Control/CharacterEnums.h>
#include <Ryu/Events/EventEnums.h>

//#include "aircarft.h"
#include <Ryu/Core/Category.h>
#include <Ryu/Core/Command.h>
#include <Ryu/Core/CommandQueue.h>
#include <Ryu/Character/CharacterIchi.h>

class CharacterIchi;

// command-actions
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
std::function<void(SceneNode&, sf::Time)> derivedEInput(Function fn)
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
PlayerController::onNotify(const SceneNode& entity, Event event)
{
    std::cout << "received change for characterspeed" << std::endl;
    switch (event)
    {
        case Event::CharacterSpeedChanged:
        {
            setActionBindingCharacterSpeed();
            break; 
        }
            
        default:
            break;
        }
}


void
PlayerController::setActionBindingCharacterSpeed()
{
    // TODO: check if this is also called correctly if we change the characterspeed
    float characterSpeed = playerCharacter->getCharacterSpeed();

    mActionBindingPress[EInput::PressLeft].action = derivedEInput<CharacterIchi>(
            CharacterMover(-characterSpeed,0.f));
    mActionBindingPress[EInput::PressRight].action = derivedEInput<CharacterIchi>(
            CharacterMover(characterSpeed,0.f));
    mActionBindingPress[EInput::PressUp].action = derivedEInput<CharacterIchi>(
            CharacterMover(0.f,-characterSpeed));
    mActionBindingPress[EInput::PressDown].action = derivedEInput<CharacterIchi>(
            CharacterMover(0.f,characterSpeed));
}

void
PlayerController::initializeBindings()
{
    mKeyBindingPress[sf::Keyboard::Left] = EInput::PressLeft;
    mKeyBindingPress[sf::Keyboard::A] = EInput::PressLeft;
    mKeyBindingPress[sf::Keyboard::Right] = EInput::PressRight;
    mKeyBindingPress[sf::Keyboard::D] = EInput::PressRight;
    mKeyBindingPress[sf::Keyboard::Up] = EInput::PressUp;
    mKeyBindingPress[sf::Keyboard::W] = EInput::PressUp;
    mKeyBindingPress[sf::Keyboard::Down] = EInput::PressDown;
    mKeyBindingPress[sf::Keyboard::S] = EInput::PressDown;

    mKeyBindingRelease[sf::Keyboard::Left] = EInput::ReleaseLeft;
    mKeyBindingRelease[sf::Keyboard::A] = EInput::ReleaseLeft;
    mKeyBindingRelease[sf::Keyboard::Right] = EInput::ReleaseRight;
    mKeyBindingRelease[sf::Keyboard::D] = EInput::ReleaseRight;
    mKeyBindingRelease[sf::Keyboard::Up] = EInput::ReleaseUp;
    mKeyBindingRelease[sf::Keyboard::W] = EInput::ReleaseUp;
    mKeyBindingRelease[sf::Keyboard::Down] = EInput::ReleaseDown;
    mKeyBindingRelease[sf::Keyboard::S] = EInput::ReleaseDown;

    setActionBindingCharacterSpeed();

    mActionBindingRelease[EInput::ReleaseLeft].action = derivedEInput<CharacterIchi>(
            CharacterMover(0.f,0.f));
    mActionBindingRelease[EInput::ReleaseRight].action = derivedEInput<CharacterIchi>(
            CharacterMover(0.f,0.f));
    mActionBindingRelease[EInput::ReleaseUp].action = derivedEInput<CharacterIchi>(
            CharacterMover(0.f,0.f));
    mActionBindingRelease[EInput::ReleaseDown].action = derivedEInput<CharacterIchi>(
            CharacterMover(0.f,0.f));

    for(auto& actionBinding : mActionBindingPress,mActionBindingRelease)
    {
        actionBinding.second.category = static_cast<unsigned>(Category::Type::Player); 
    }
}

PlayerController::PlayerController(CharacterIchi* character)
: Observer("Playercontroller")
, playerCharacter(character)
{
    
    initializeBindings();
}

void
PlayerController::handleEvent(const sf::Event& event, CommandQueue& commands)
{
   /* 
   * TODO
   * use a binding map from EInput to functioncall
   * 
   */

    //std::cout << "PlayerHandleEvent " << event.type << std::endl;
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
                //std::cout << "Right pressed " << std::endl;
                // TODO: send sf::events (release/press and which key to pc!!! -)
                playerCharacter->handleInput(EInput::PressRight);
                break;
            }

            case sf::Keyboard::A:
            case sf::Keyboard::Left:
            {
                //std::cout << "Left pressed " << std::endl;
                playerCharacter->handleInput(EInput::PressLeft);
                break;
            }

            case sf::Keyboard::W:
            case sf::Keyboard::Up:
            {
                //std::cout << "Left pressed " << std::endl;
                playerCharacter->handleInput(EInput::PressUp);
                break;
            }

            case sf::Keyboard::S:
            case sf::Keyboard::Down:
            {
                //std::cout << "Left pressed " << std::endl;
                playerCharacter->handleInput(EInput::PressDown);
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
                //std::cout << "Right released " << std::endl;
                playerCharacter->handleInput(EInput::ReleaseRight);
                commands = {};
                commands.push(mActionBindingRelease[EInput::ReleaseRight]);
                break;
            }
            case sf::Keyboard::A:
            case sf::Keyboard::Left:
            {
                //std::cout << "Left released " << std::endl;
                playerCharacter->handleInput(EInput::ReleaseLeft);
                /* TODO: make this less boilerplate ! 
                here we need to clear all pending movement-commands 
                -> is this a problem for later commands ? maybe only remove category commands
                */
                commands = {};
                commands.push(mActionBindingRelease[EInput::ReleaseLeft]);
                break;
            }

            case sf::Keyboard::S:
            case sf::Keyboard::Down:
            {
                //std::cout << "Left pressed " << std::endl;
                playerCharacter->handleInput(EInput::ReleaseDown);
                commands = {};
                commands.push(mActionBindingRelease[EInput::ReleaseDown]);
                break;
            }

            case sf::Keyboard::W:
            case sf::Keyboard::Up:
            {
                //std::cout << "Left pressed " << std::endl;
                playerCharacter->handleInput(EInput::ReleaseUp);
                commands = {};
                commands.push(mActionBindingRelease[EInput::ReleaseUp]);
                break;
            }

            default:
                break;
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

void
PlayerController::handleRealtimeInput(CommandQueue& commands)
{
    /*
    sf::Vector2f movement(0.f, 0.f);
	if(mIsMovingUp)
		movement.y -= PlayerSpeed;
	if(mIsMovingDown)
		movement.y += PlayerSpeed;
	if(mIsMovingLeft)
		movement.x -= PlayerSpeed;
	if(mIsMovingRight)
		movement.x += PlayerSpeed;
    */


    for(auto const& binding : mKeyBindingPress)
    {
        if(sf::Keyboard::isKeyPressed(binding.first) && isRealtimeAction(binding.second))
        {
            commands.push(mActionBindingPress[binding.second]);
        }
    }    
}

bool
PlayerController::isRealtimeAction(EInput action)
{
	switch (action)
	{
		case EInput::PressLeft:
		case EInput::PressRight:
		case EInput::PressDown:
		case EInput::PressUp:
			return true;

		default:
			return false;
	}
}

void
PlayerController::assignKey(EInput action, sf::Keyboard::Key key)
{
	// Remove all keys that already map to action
	for (auto itr = mKeyBindingPress.begin(); itr != mKeyBindingPress.end(); )
	{
		if (itr->second == action)
			mKeyBindingPress.erase(itr++);
		else
			++itr;
	}

	// Insert new binding
	mKeyBindingPress[key] = action;
}

sf::Keyboard::Key
PlayerController::getAssignedKey(EInput action) const
{
    for(auto& binding : mKeyBindingPress)
    {
        if (binding.second == action)
			return binding.first;
    }
	
	return sf::Keyboard::Unknown;
}