#pragma once

#include <Ryu/Scene/EntityStatic.h>

#include <box2d/box2d.h>
#include <box2d/b2_contact.h>
#include <fmt/core.h>
#include <vector>


class RyuContactListener : public b2ContactListener
{

  public:

    RyuContactListener()
      : contactObject("")
    {
    }

    void BeginContact(b2Contact* contact)
    {
      // t.b.c

      auto userData = contact->GetFixtureA()->GetBody()->GetUserData();
      EntityStatic* entity = reinterpret_cast<EntityStatic*>(userData.pointer);

      std::vector<sf::Vector2f> cornerPoints = entity->getCornerPoints();
      entity->increaseContactPoints();

      fmt::print("Get Contact with {}, ({}) climbable: {}, points: A: {}/{} B: {}/{} C: {}/{} D: {}/{}\n"
                 , entity->getName(), entity->getContactPoints()
                 , entity->getEntityType() == EntityType::Climbable ? "Yes" : "No"
                 , cornerPoints.at(0).x, cornerPoints.at(0).y
                 , cornerPoints.at(1).x, cornerPoints.at(1).y
                 , cornerPoints.at(2).x, cornerPoints.at(2).y
                 , cornerPoints.at(3).x, cornerPoints.at(3).y
                 );

      contactObject = entity->getName();
    }

    void EndContact(b2Contact* contact)
    {

      auto userData = contact->GetFixtureA()->GetBody()->GetUserData();
      EntityStatic* entity = reinterpret_cast<EntityStatic*>(userData.pointer);
      entity->decreaseContactPoints();
      fmt::print("End Contact with {} ({})\n", entity->getName(), entity->getContactPoints());
      contactObject = "";
    }

    std::string contactObject;
};
