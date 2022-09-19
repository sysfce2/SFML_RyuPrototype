#include "scenenode.h"
#include <algorithm>
#include <cassert>

SceneNode::SceneNode()
: mChildren()
, mParent(nullptr)
{
}

void
SceneNode::attachChild(Ptr child)
{
    child->mParent = this;
    mChildren.push_back(std::move(child));
}

SceneNode::Ptr
SceneNode::detachChild(const SceneNode& node)
{
    // search element with STDL & lamda
    auto found = std::find_if(mChildren.begin(), mChildren.end(),
    [&](Ptr& p) -> bool { return p.get() == &node; });

    assert (found!=mChildren.end());

    // move foundNode in a variable
    Ptr result = std::move(*found);
    // lonewolf
    result->mParent = nullptr;
    mChildren.erase(found);
    return result;
}

void
SceneNode::draw(sf::RenderTarget& target,
                sf::RenderStates states) const
{
    // chain absolute Transform with current node relative Transform
    states.transform *= getTransform();

    // draw derived object
    drawCurrent(target, states);

    // draw child nodes
    for (const Ptr& child: mChildren)
    {
        child->draw(target, states);
    }

}

void
SceneNode::drawCurrent(sf::RenderTarget&, sf::RenderStates) const
{
	// Do nothing by default
}

 void 
 SceneNode::update(sf::Time dt)
 {
     updateCurrent(dt);
     updateChildren(dt);
 }

 void
 SceneNode::updateCurrent(sf::Time dt)
 {}

 void
 SceneNode::updateChildren(sf::Time dt)
 {
     for(const auto& child: mChildren)
     {
         child->update(dt);
     }
 }

 sf::Transform
 SceneNode::getWorldTransform() const
 {
     sf::Transform transform = sf::Transform::Identity;

     for(const SceneNode* node = this; node != nullptr; node = node->mParent)
     {
         transform = node->getTransform() * transform;
     }

     return transform;
 }

sf::Vector2f
SceneNode::getWorldPosition() const
{
    return getWorldTransform() * sf::Vector2f();
}