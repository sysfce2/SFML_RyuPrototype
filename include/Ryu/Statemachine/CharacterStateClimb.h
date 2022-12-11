#pragma once

#include "CharacterState.h"
#include <Ryu/Control/CharacterEnums.h>
#include <memory>

class CharacterStateClimb : public CharacterState
{
    public:
        CharacterStateClimb();
        ~CharacterStateClimb();
        std::unique_ptr<CharacterState> handleInput(CharacterBase& character,EInput input) override;
        void update(CharacterBase& character) override;
    
        void enter(CharacterBase& character) override;
        void exit(CharacterBase& character) override;
    private:
				EClimbHeight mClimbHeight;
		
};