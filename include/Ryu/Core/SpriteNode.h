#pragma once

#include <SFML/Graphics.hpp>
#include <Ryu/Scene/SceneNode.h>

//namespace ryu {
class SpriteNode : public SceneNode
{
    public:
        explicit SpriteNode(const sf::Texture& texture);
        SpriteNode(const sf::Texture& texture, const sf::IntRect& rect);

    private:
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

        sf::Sprite mSprite;
};
//} ///namespace ryu