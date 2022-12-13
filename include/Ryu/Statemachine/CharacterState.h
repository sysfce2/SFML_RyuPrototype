#pragma once

#include <Ryu/Control/CharacterEnums.h>

#include <memory>

//namespace ryu{

class CharacterBase;

class CharacterState
{
    public:
        virtual ~CharacterState(){};
        virtual std::unique_ptr<CharacterState> handleInput(CharacterBase& character,EInput input) = 0;
        virtual void update(CharacterBase& character) = 0;
        virtual void enter(CharacterBase& character) = 0;
        virtual void exit(CharacterBase& character) = 0;
};

//} /// namespace ryu