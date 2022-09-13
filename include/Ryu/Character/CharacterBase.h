#pragma once

#include <SFML/Graphics.hpp>
#include <Ryu/Statemachine/CharacterState.h>
#include <Ryu/Core/AssetManager.h>

using namespace ryu;

namespace ryu{

enum class EMoveDirecton
{
	NONE,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	UPLEFT,
	UPRIGHT,
	DOWNLEFT,
	DOWNRIGHT
};

class CharacterBase {

    public:
        // TODO: implement rule of 5 !
        // (morph one character into another ^^)
        CharacterBase();
        ~CharacterBase();

        virtual void handleInput(EInput input);
        virtual void update(sf::Time deltaTime);

        void setTexture(AssetManager<sf::Texture, Textures::ID> &textureManager, Textures::ID id);
        sf::Drawable& getSprite() { return mPlayer;}

        void changeColor(sf::Color color);

// TODO: make private/setter and make Vector or st ? -> see Game.cpp
        bool mIsMovingUp=false;
        bool mIsMovingDown=false;
        bool mIsMovingLeft=false;
        bool mIsMovingRight=false;

    protected:
        // TODO: use smart pointers instead ?!!! and move semantics
        CharacterState* characterState;

    private:
        sf::Sprite mPlayer;
        EMoveDirecton mMoveDirection;
        

        float PlayerSpeed = 75.f;

};

} /// namespace ryu