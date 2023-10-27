#pragma once

#include "CharacterState.h"
#include <memory>

class CharacterStateRun : public CharacterState
{
    public:
        CharacterStateRun();
        ~CharacterStateRun();
        std::unique_ptr<CharacterState> handleInput(CharacterBase& character,EInput input) override;
        void update(CharacterBase& character) override;
    
        void enter(CharacterBase& character) override;
        void exit(CharacterBase& character) override;
        void onNotify(CharacterBase &character, Ryu::EEvent event) override {};
    private:
        float mLastCharacterSpeed;
        float mRunCharacterSpeed;
};
