#pragma once

#include <Ryu/Scene/SceneNode.h>
#include <Ryu/Core/Category.h>
#include <SFML/Graphics.hpp>

//namespace ryu {
class Entity : public SceneNode
{
    public:
        // t.b.c
        virtual ~Entity(); 
        void setVelocity(sf::Vector2f velocity);
        void setVelocity(float vx, float vy);
        sf::Vector2f getVelocity() const;
        unsigned int getCategory() const override { return static_cast<unsigned>(Category::Type::None);}

    private:
        virtual void updateCurrent(sf::Time dt);
        sf::Vector2f mVelocity;
        
};
//} /// namespace ryu