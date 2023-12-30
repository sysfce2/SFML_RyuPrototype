#pragma once

//#include <Ryu/Scene/SceneNode.h>
#include <Ryu/Core/Category.h>
#include <SFML/Graphics.hpp>
#include <fmt/core.h>
#include <memory>
#include <vector>

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
        EntityType getEntityType() {return mEntityType;};
        void setShape(sf::Shape* shape);
        sf::Shape* getShape(){
                return mShape;
        };
        void setContact(bool isContact) {mIsInContact = isContact;};
        void setCornerPoints(std::vector<sf::Vector2f> points);
        std::vector<sf::Vector2f> getCornerPoints();
        std::string getName();
        void setName(std::string name);

private:
        bool mIsInContact;
        virtual void updateCurrent(sf::Time dt);
        sf::Shape* mShape;
        EntityType mEntityType;
        std::vector<sf::Vector2f> mBorderPoints;
        std::string mName;


};
//} /// namespace ryu_
