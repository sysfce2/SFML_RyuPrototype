#pragma once

#include "CharacterState.h"

#include <SFML/System/Clock.hpp>
#include <memory>


class CharacterStateFallingEnd : public CharacterState
{
    public:
        CharacterStateFallingEnd();
        ~CharacterStateFallingEnd();
        std::unique_ptr<CharacterState> handleInput(CharacterBase& character,EInput input) override;
        void update(CharacterBase& character) override;
    
        void enter(CharacterBase& character) override;
        void exit(CharacterBase& character) override;
        void onNotify(CharacterBase &character, Ryu::EEvent event) override {};
    private:
        bool touchedFloor;
    
};
