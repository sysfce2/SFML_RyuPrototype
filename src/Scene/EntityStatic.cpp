#include <Ryu/Scene/EntityStatic.h>

//namespace ryu {

EntityStatic::EntityStatic(EntityType type) :
    mShape(nullptr)
    ,mEntityType(type)
    ,mIsInContact(false)
{}
EntityStatic::~EntityStatic() {}

void
EntityStatic::setShape(sf::Shape* shape)
{
  if(mShape != nullptr) mShape = shape;
}

void
EntityStatic::updateCurrent(sf::Time dt)
{
}

void
EntityStatic::setCornerPoints(std::vector<sf::Vector2f> points)
{
  mBorderPoints = points;
}

std::vector<sf::Vector2f>
EntityStatic::getCornerPoints()
{
  return mBorderPoints;
}

std::string
EntityStatic::getName()
{
  return mName;
}

void
EntityStatic::setName(std::string name)
{
  mName = name;
}
//} /// namespace ryu
