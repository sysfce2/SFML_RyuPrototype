#include <Ryu/Scene/EntityStatic.h>

//namespace ryu {

EntityStatic::EntityStatic(EntityType type) :
    shape(nullptr)
    ,entityType(type)
{}
EntityStatic::~EntityStatic() {}

void
EntityStatic::setShape(sf::Shape* _shape)
{
  if(_shape != nullptr) shape = _shape;
}

void
EntityStatic::updateCurrent(sf::Time dt)
{
}

//} /// namespace ryu
