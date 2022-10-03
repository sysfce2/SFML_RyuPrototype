#pragma once

#include <map>
#include <memory>
#include <SFML/Window/Keyboard.hpp>

class CommandQueue;
class Command;
class CharacterIchi;

namespace sf{
    class Event;
}

class PlayerController
{
    public:
        enum class Action
        {
            MoveLeft,
            MoveRight,
            MoveUp,
            MoveDown
        };

    public:

        PlayerController(std::unique_ptr<CharacterIchi> const &character);

        void assignKey(Action action, sf::Keyboard::Key key);
        sf::Keyboard::Key getAssignedKey(Action action) const;

        void handleEvent(const sf::Event& event,
                         CommandQueue& commands);

        void handleRealtimeInput(CommandQueue& commands);

    private:
        static bool isRealtimeAction(Action action);
        void initializeBindings();

        std::map<sf::Keyboard::Key, Action> mKeyBinding;
        std::map<Action, Command> mActionBinding;

        std::unique_ptr<CharacterIchi> const &playerCharacter;

};