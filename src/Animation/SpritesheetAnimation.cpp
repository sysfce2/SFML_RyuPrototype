
#include <iostream>
#include <Ryu/Animation/SpritesheetAnimation.h>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>


SpritesheetAnimation::SpritesheetAnimation()
: mSprite()
, mFrameSize()
, mNumFrames(0)
, mCurrentFrame(0)
, mDuration(sf::Time::Zero)
, mElapsedTime(sf::Time::Zero)
, mRepeat(false)
, mStartFrame({0,0})
{}

SpritesheetAnimation::SpritesheetAnimation(const sf::Texture& texture)
: mSprite(texture)
, mFrameSize()
, mNumFrames(0)
, mCurrentFrame(0)
, mDuration(sf::Time::Zero)
, mElapsedTime(sf::Time::Zero)
, mRepeat(false)
, mStartFrame({0,0})
{}

void SpritesheetAnimation::setTexture(const sf::Texture& texture)
{
	mSprite.setTexture(texture);
}

const sf::Texture* SpritesheetAnimation::getTexture() const
{
	return mSprite.getTexture();
}

void SpritesheetAnimation::setFrameSize(sf::Vector2i frameSize)
{
	mFrameSize = frameSize;
}

sf::Vector2i SpritesheetAnimation::getFrameSize() const
{
	return mFrameSize;
}

void SpritesheetAnimation::setNumFrames(std::size_t numFrames)
{
	mNumFrames = numFrames;
}

std::size_t SpritesheetAnimation::getNumFrames() const
{
	return mNumFrames;
}

void SpritesheetAnimation::setDuration(sf::Time duration)
{
	mDuration = duration;
}

sf::Time SpritesheetAnimation::getDuration() const
{
	return mDuration;
}

void SpritesheetAnimation::setRepeating(bool flag)
{
	mRepeat = flag;
}

bool SpritesheetAnimation::isRepeating() const
{
	return mRepeat;
}

void SpritesheetAnimation::restart()
{
	mCurrentFrame = 0;
}

bool SpritesheetAnimation::isFinished() const
{
	return mCurrentFrame >= mNumFrames;
}

sf::FloatRect SpritesheetAnimation::getLocalBounds() const
{
	return sf::FloatRect(getOrigin(), static_cast<sf::Vector2f>(getFrameSize()));
}

sf::FloatRect SpritesheetAnimation::getGlobalBounds() const
{
	return getTransform().transformRect(getLocalBounds());
}

void 
SpritesheetAnimation::setStartFrame(sf::Vector2i startFrame)
{
    mStartFrame = startFrame;
}


void 
SpritesheetAnimation::update(sf::Time dt)
{
    /* Tasks:
    * X1. start ani with specific frame on spritesheet
    * 2. Time per frame
    * 3. get movedirection / mirrow spritesheet IF sprite is morrowable
    */
    
    sf::Time timePerFrame = mDuration / static_cast<float>(mNumFrames);
    mElapsedTime += dt;
    sf::Vector2i textureBounds(mSprite.getTexture()->getSize());
    sf::IntRect textureRect = mSprite.getTextureRect();
    if (mCurrentFrame == 0)
    {
        textureRect = sf::IntRect(mStartFrame.x, mStartFrame.y, mFrameSize.x, mFrameSize.y);
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
                textureRect = sf::IntRect(mStartFrame.x, mStartFrame.y, mFrameSize.x,mFrameSize.y);
            }
        }
        else
        {
            mCurrentFrame++;
        }
    }
    
    mSprite.setTextureRect(textureRect); 
}

void
SpritesheetAnimation::flipAnimationX(EMoveDirecton moveDir)
{
    /*
    if(moveDir == EMoveDirecton::Left)
    {
        // flip X
        mSprite.setTextureRect
        (sf::IntRect(
            mSprite.getTextureRect().width, 
            0, 
            -(mSprite.getTextureRect().width), 
            mSprite.getTextureRect().height)
        );
    }
    else
    {
        mSprite.setTextureRect
        (sf::IntRect(
            mSprite.getTextureRect().width, 
            0, 
            (mSprite.getTextureRect().width), 
            mSprite.getTextureRect().height)
        );
    }
    */
}

void SpritesheetAnimation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  	states.transform *= getTransform();
	target.draw(mSprite, states);
}