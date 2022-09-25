#pragma once

#include "CharacterState.h"
#include <memory>

using namespace ryu;

class CharacterStateIdle : public CharacterState
{
    public:
        CharacterStateIdle();
        ~CharacterStateIdle();
        std::unique_ptr<CharacterState> handleInput(CharacterBase& character,EInput input) override;
        void update(CharacterBase& character) override;

        void enter(CharacterBase& character) override;
        void exit(CharacterBase& character) override;
    
};