#pragma once

#include "CharacterState.h"
#include <memory>

class CharacterStateLedgeClimbUp : public CharacterState
{
    public:
        CharacterStateLedgeClimbUp();
        ~CharacterStateLedgeClimbUp();
        std::unique_ptr<CharacterState> handleInput(CharacterBase& character,EInput input) override;
        void update(CharacterBase& character) override;

        void enter(CharacterBase& character) override;
        void exit(CharacterBase& character) override;
        void onNotify(CharacterBase &character, Ryu::EEvent event) override {};

};
