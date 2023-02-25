#pragma once

#include <Ryu/Control/CharacterEnums.h>
#include <Ryu/Events/EventEnums.h>

#include <SFML/Graphics.hpp>

struct AnimationFrame
{
  float duration; // in seconds
  sf::Vector2i frameSize;
	EEvent event;
};

class SpritesheetAnimation : public sf::Drawable, public sf::Transformable
{
    public:
        SpritesheetAnimation();
	  	  explicit SpritesheetAnimation(const sf::Texture& texture);

		    void setTexture(const sf::Texture& texture);
		    const sf::Texture* getTexture() const;

		    void setFrameSize(sf::Vector2i mFrameSize);
		    sf::Vector2i getFrameSize() const;

		    void setNumFrames(std::size_t numFrames);
		    std::size_t getNumFrames() const;
		    std::size_t getFramesCount() const {return mFrames.size();}

		    void setStartFrame(sf::Vector2i startFrame);

		    void setDuration(sf::Time duration);
		    sf::Time getDuration() const;

		    void setRepeating(bool flag);
		    bool isRepeating() const;

		    void restart();
		    bool isFinished() const;

		    void flipAnimationLeft();
		    void flipAnimationRight();

		    sf::FloatRect getLocalBounds() const;
		    sf::FloatRect getGlobalBounds() const;
        void update(sf::Time dt);
		    sf::Sprite& getSprite() { return mSprite;}
        
	private:
		    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

  private:
	  	  sf::Sprite mSprite;
        sf::Vector2i mFrameSize;
		    sf::Vector2i mStartFrame;
        std::size_t mNumFrames;
        std::size_t mCurrentFrame;
        sf::Time mDuration;
        sf::Time mElapsedTime;
        bool mRepeat;
        std::vector<AnimationFrame> mFrames;
};  