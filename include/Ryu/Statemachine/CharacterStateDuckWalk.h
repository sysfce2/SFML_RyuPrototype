#pragma once

#include "CharacterState.h"
#include <memory>

class CharacterStateDuckWalk : public CharacterState
{
    public:
        CharacterStateDuckWalk();
        ~CharacterStateDuckWalk();
        std::unique_ptr<CharacterState> handleInput(CharacterBase& character,EInput input) override;
        void update(CharacterBase& character) override;

        void enter(CharacterBase& character) override;
        void exit(CharacterBase& character) override;

};