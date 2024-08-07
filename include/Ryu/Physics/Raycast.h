#pragma once

#include <Ryu/Core/Utilities.h>
#include <Ryu/Events/Subject.h>
#include <Ryu/Physics/RaycastTypes.h>

#include <box2d/b2_world_callbacks.h>
#include <box2d/b2_math.h>
#include <fmt/core.h>

#include <iostream>
#include <memory>

class SceneNode;

/*
*  This callback finds the closest hit.
*/
class RayCastClosest : public b2RayCastCallback
{
  public:
    RayCastClosest() : 
      m_Hit(false),
      owner(nullptr),
      m_fraction(0)
    {}

    float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override
    {
      // TODO: check if fixture with index 1 needs to be ignored like in the testbed test, see fr details [b2d_testbed]tests/ray_cast.cpp

      m_Hit = true;
      m_Point = point;
      m_Normal = normal;

      if(owner != nullptr)
      {
        std::cout << "owner\n";
      }

      // By returning the current fraction, we instruct the calling code to clip the ray and
  		// continue the ray-cast to the next fixture. WARNING: do not assume that fixtures
  		// are reported in order. However, by clipping, we can always get the closest fixture.
	  	m_fraction = fraction;
      return fraction;
    }

    float getFraction() {return m_fraction;}

    b2Vec2 getHitPoint() {return m_Point;}

    void setOwner(std::unique_ptr<SceneNode> raycastOwner)
    {
      owner = std::move(raycastOwner);
    }

    std::unique_ptr<SceneNode> owner;

    bool m_Hit;
    b2Vec2 m_Point;
    b2Vec2 m_Normal;
    float m_fraction;
};

// see ray_cast.cpp : ln. 375 flwd. creates raycast ....

