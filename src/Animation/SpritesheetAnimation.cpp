
#include <Ryu/Animation/SpritesheetAnimation.h>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <iostream>
#include <vector>

SpritesheetAnimation::SpritesheetAnimation()
: mSprite()
, mFrameSize()
, mNumFrames(0)
, mCurrentFrame(0)
, mDuration(sf::Time::Zero)
, mElapsedTime(sf::Time::Zero)
, mRepeat(false)
, mStartFrame({0,0})
, mFrames({})
{
}

SpritesheetAnimation::SpritesheetAnimation(const sf::Texture& texture)
: mSprite(texture)
, mFrameSize()
, mNumFrames(0)
, mCurrentFrame(0)
, mDuration(sf::Time::Zero)
, mElapsedTime(sf::Time::Zero)
, mRepeat(false)
, mStartFrame({0,0})
, mFrames({})
{    // set origin of texture to center
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    std::cout << "Boundswidth: " << bounds.width << "Boundsheight: " << bounds.height << "\n";
    //mSprite.setOrigin(120.f, 144.f);
}

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



void SpritesheetAnimation::setNumFrames(std::size_t numFrames/*,sf::Time standardDuration*/ )
{
	mNumFrames = numFrames;

	/* TODO: 	what if we want to extend the animation afterwards, 
						we dont want to recreate all the timing for every frame...
	*/
 /*
	if(numFrames > mFrames.size())
	{
		mFrames.push_back(
			{
				.duration= 100,
				.frameSize = {80,96},
				.event = Event::None
			}
		);
	}
	else
	{
	*/
		mFrames.clear();
	 for(int i=0;i<numFrames;++i)
	 {
		AnimationFrame frame{
			.duration= 100,
			.frameSize = {80,96},
			.event = Event::None
		};
			mFrames.push_back(frame);	
	 }
	//}
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
		* 1b send signal when animation is finished
		* 1c. make Frame a struct
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

    //std::cout << "texRec:" << textureRect.width << "," << textureRect.height << "\n";
    
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
    //std::cout << "TextureRect: " << textureRect.width << "," << textureRect.height << "\n"; 
    mSprite.setTextureRect(textureRect); 
}

void
SpritesheetAnimation::flipAnimationLeft()
{   
    if(mSprite.getScale().x != -1)
    {
        // do not ever use mSprite.scale({x,y}) !!!
        mSprite.setScale({-1,1});
        std::cout << "Flipx LEFT" << "\n";
    }   
}

void
SpritesheetAnimation::flipAnimationRight()
{   
    if(mSprite.getScale().x != 1)
    {
        // do not ever use mSprite.scale({x,y}) !!!     
        mSprite.setScale({1,1});
        std::cout << "Flipx Right" << "\n";
    }
}
    
void SpritesheetAnimation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  	states.transform *= getTransform();
		target.draw(mSprite, states);
}