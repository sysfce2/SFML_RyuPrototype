#pragma once

#include "SceneNode.h"
#include <SFML/Graphics.hpp>

class Entity : public SceneNode
{
    public:
        // t.b.c
    private:
        virtual void updateCurrent(sf::Time dt);
        
};