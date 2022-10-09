#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

/*
*   this is a alternative animation class took from th SFML-wiki.
*   with this approach we can start a animation at a certain position 
*   in the spritesheet and are more flexible with timings for every frame
*   looping,rewind or play animation X times needs to be added
*/

struct Frame {
   sf::IntRect rect;
   double duration; // in seconds
};

enum class modes
{
   Single,
   Loop
};

class Animation {
   public:
    explicit Animation(sf::Sprite &target);
    virtual ~Animation() {};
    void addFrame(Frame&& frame);
    void update(double elapsed); 
    const double getLength() const { return totalLength; }
    sf::Sprite& getSprite() {return target;}

 private:
    std::vector<Frame> frames;
    double totalLength;
    double progress;
    sf::Sprite &target;
    modes playbackMode;

};