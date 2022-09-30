#pragma once

#include <memory>

//namespace ryu{

class CharacterBase;

// TODO: move to inputcontroller or enumfile
enum class EInput 
{
    NONE,
    PRESSUP,
    PRESSDOWN,
    PRESSRIGHT,
    PRESSLEFT,
    RELEASEUP,
    RELEASEDOWN,
    RELEASERIGHT,
    RELEASELEFT,
};

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