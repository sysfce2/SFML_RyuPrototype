#include "scenenode.h"
#include <algorithm>
#include <cassert>

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