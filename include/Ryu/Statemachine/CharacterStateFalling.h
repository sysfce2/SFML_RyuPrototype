#pragma once

#include "CharacterState.h"

#include <SFML/System/Clock.hpp>
#include <memory>


class CharacterStateFalling : public CharacterState
{
    public:
        CharacterStateFalling();
        ~CharacterStateFalling();
        std::unique_ptr<CharacterState> handleInput(CharacterBase& character,EInput input) override;
        void update(CharacterBase& character) override;
    
        void touchFloor(CharacterBase& character);

        void enter(CharacterBase& character) override;
        void exit(CharacterBase& character) override;
    private:
        bool touchedFloor;
        sf::Clock timer;
        float timerTimeInMs;
    
};