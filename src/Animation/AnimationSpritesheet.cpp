/*
    Template for SpritesheetAnimation ! NOT USED !
*/


#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <Ryu/Animation/AnimationSpritesheet.h>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>


AnimationSpritesheet::AnimationSpritesheet()
: mSprite()
, mFrameSize()
, mNumFrames(0)
, mCurrentFrame(0)
, mDuration(sf::Time::Zero)
, mElapsedTime(sf::Time::Zero)
, mRepeat(false)
, mStartFrame(0)
{}

AnimationSpritesheet::AnimationSpritesheet(const sf::Texture& texture)
: mSprite(texture)
, mFrameSize()
, mNumFrames(0)
, mCurrentFrame(0)
, mDuration(sf::Time::Zero)
, mElapsedTime(sf::Time::Zero)
, mRepeat(false)
, mStartFrame(0)
{}

void AnimationSpritesheet::setTexture(const sf::Texture& texture)
{
	mSprite.setTexture(texture);
}

const sf::Texture* AnimationSpritesheet::getTexture() const
{
	return mSprite.getTexture();
}

void AnimationSpritesheet::setFrameSize(sf::Vector2i frameSize)
{
	mFrameSize = frameSize;
}

sf::Vector2i AnimationSpritesheet::getFrameSize() const
{
	return mFrameSize;
}

void AnimationSpritesheet::setNumFrames(std::size_t numFrames)
{
	mNumFrames = numFrames;
}

std::size_t AnimationSpritesheet::getNumFrames() const
{
	return mNumFrames;
}

void AnimationSpritesheet::setDuration(sf::Time duration)
{
	mDuration = duration;
}

sf::Time AnimationSpritesheet::getDuration() const
{
	return mDuration;
}

void AnimationSpritesheet::setRepeating(bool flag)
{
	mRepeat = flag;
}

bool AnimationSpritesheet::isRepeating() const
{
	return mRepeat;
}

void AnimationSpritesheet::restart()
{
	mCurrentFrame = 0;
}

bool AnimationSpritesheet::isFinished() const
{
	return mCurrentFrame >= mNumFrames;
}

sf::FloatRect AnimationSpritesheet::getLocalBounds() const
{
	return sf::FloatRect(getOrigin(), static_cast<sf::Vector2f>(getFrameSize()));
}

sf::FloatRect AnimationSpritesheet::getGlobalBounds() const
{
	return getTransform().transformRect(getLocalBounds());
}

void 
AnimationSpritesheet::setStartFrame(std::size_t startFrame)
{
    mStartFrame = startFrame;
}


void 
AnimationSpritesheet::update(sf::Time dt)
{
    sf::Time timePerFrame = mDuration / static_cast<float>(mNumFrames);
    mElapsedTime += dt;
    sf::Vector2i textureBounds(mSprite.getTexture()->getSize());
    sf::IntRect textureRect = mSprite.getTextureRect();
    if (mCurrentFrame == 0)
    {
        textureRect = sf::IntRect(0, 0, mFrameSize.x, mFrameSize.y);
    }
    
    while (mElapsedTime >= timePerFrame && (mCurrentFrame <= mNumFrames || mRepeat))
    {
        textureRect.left += textureRect.width;
        if (textureRect.left + textureRect.width > textureBounds.x)
        {
            textureRect.left = 0;
            textureRect.top += textureRect.height;
        }
        mElapsedTime -= timePerFrame;
        if (mRepeat)
        {
            mCurrentFrame = (mCurrentFrame + 1) % mNumFrames;
            if (mCurrentFrame == 0)
            {
                textureRect = sf::IntRect(0, 0, mFrameSize.x,mFrameSize.y);
            }
        }
        else
        {
            mCurrentFrame++;
        }
    }
    
    mSprite.setTextureRect(textureRect); 
}

void AnimationSpritesheet::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  	states.transform *= getTransform();
	target.draw(mSprite, states);
}
