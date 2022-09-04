#pragma once

#include "CharacterState.h"


using namespace ryu;

class CharacterStateRun : public CharacterState
{
    public:
        CharacterStateRun();
        ~CharacterStateRun();
        CharacterState* handleInput(CharacterBase& character,EInput input) override;
        void update(CharacterBase& character) override;
    
        void enter(CharacterBase& character) override;
        void exit(CharacterBase& character) override;
};