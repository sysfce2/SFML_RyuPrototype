#pragma once

//#include <Ryu/Scene/SceneNode.h>
#include <Ryu/Core/Category.h>
#include <SFML/Graphics.hpp>
#include <memory>

enum class EntityType {
    None = 0,
    Climbable = 1,
};

//namespace ryu {
class EntityStatic //: public SceneNode
{
    public:
        // t.b.c
        EntityStatic(EntityType type);
        virtual ~EntityStatic();
        unsigned int getCategory() const { return static_cast<unsigned>(Category::Type::None);}
        EntityType getEntityType() {return entityType;};
        void setShape(sf::Shape* shape);
        sf::Shape* getShape(){return shape;};

    private:
        virtual void updateCurrent(sf::Time dt);
        sf::Shape* shape;
        EntityType entityType;


};
//} /// namespace ryu_
