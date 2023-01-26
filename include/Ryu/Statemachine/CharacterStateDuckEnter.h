#pragma once

#include "CharacterState.h"
#include <memory>

class CharacterStateDuckEnter : public CharacterState
{
    public:
        CharacterStateDuckEnter();
        ~CharacterStateDuckEnter();
        std::unique_ptr<CharacterState> handleInput(CharacterBase& character,EInput input) override;
        void update(CharacterBase& character) override;

        void enter(CharacterBase& character) override;
        void exit(CharacterBase& character) override;

};