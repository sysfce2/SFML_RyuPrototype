#include "Ryu/Control/CharacterEnums.h"
#include "Ryu/Debug/b2DrawSFML.hpp"
#include "Ryu/Physics/RaycastTypes.h"
#include <Ryu/Physics/RaycastComponent.h>
#include <Ryu/Scene/SceneNode.h>
#include <box2d/box2d.h>
//#include <Ryu/Physics/Raycast.h>
//#include <Ryu/Physics/RaycastTypes.h>

namespace Ryu::Physics {
RaycastComponent::RaycastComponent() : rayCastCallbacks() {}

bool
RaycastComponent::getHit(RaycastPosition rcName)
{
    if (rayCastCallbacks.find(rcName) != rayCastCallbacks.end())
        return rayCastCallbacks.at(rcName).m_Hit;

    return false;
}

void
RaycastComponent::eraseRaycast(RaycastPosition rcName)
{
    rayCastPoints.erase(rcName);
    eraseRaycastPoints(rcName);
}

void
RaycastComponent::eraseRaycastPoints(RaycastPosition rcName)
{
    rayCastCallbacks.erase(rcName);
}

void
RaycastComponent::drawRaycasts(b2DrawSFML &debugDrawer)
{

    for (auto rc : rayCastPoints)
    {
        debugDrawer.DrawSegment(rc.second.first, rc.second.second,
                                b2HexColor::b2_colorBox2DRed);
    }
}

void
RaycastComponent::eraseBelow()
{
    if (rayCastCallbacks.find(RaycastPosition::Below) != rayCastCallbacks.end())
        eraseRaycast(RaycastPosition::Below);
}

void
RaycastComponent::createCharacterRaycast(RaycastPosition position,
                                         float rcPositionX, float rcPositionY,
                                         float angle, float length,
                                         EMoveDirection moveDirection,
                                         std::unique_ptr<b2WorldId> &physWorld)
{
    rayCastCallbacks[position] = Ryu::Physics::createRaycast(
        position, std::make_pair(rcPositionX, rcPositionY), angle, length,
        moveDirection, physWorld, rayCastPoints);
}

// TODO: imo this returns a copy of the raycast propably this is unefficient
RayCastClosest
createRaycast(RaycastPosition localPosition,
              std::pair<double, double> startPoint, float angle, float length,
              EMoveDirection charMoveDirection,
              std::unique_ptr<b2WorldId> &physWorld, RaycastPoints &rayCastPoints)
{
    // creating a raycast from the characters position downwards
    // 0° right / 90° up / 180° left / 270° down
    // TODO: revise raycasting in Box2D v3.1.
    float raycastAngle = B2_PI * angle / 180.0f;
    float lengthMeter = Converter::pixelsToMeters<double>(length);
    b2Vec2 d(lengthMeter * cosf(raycastAngle),
             lengthMeter * sinf(raycastAngle));

    // direction according lookdir of character
    bool rcLeftOrRight = (angle >= 180 || angle == 0);
    int dir
        = ((charMoveDirection == EMoveDirection::Left && rcLeftOrRight) ? -1
                                                                        : 1);

    b2Vec2 p1(Converter::pixelsToMeters<double>(startPoint.first),
              Converter::pixelsToMeters<double>(startPoint.second));

    b2Vec2 p2 = p1 + (dir * d);
    auto rc = rayCastPoints.find(localPosition);

    if (rc != rayCastPoints.end())
    {
        rc->second.first = p1;
        rc->second.second = p2;
    } else
    {
        rayCastPoints.insert(
            std::make_pair(localPosition, std::make_pair(p1, p2)));
    }

    RayCastClosest callback;
    // TODO. create raycast from worldid
    // now with origin and translation not start and end
    //physWorld.get()->RayCast(&callback, p1, p2);

    return callback;
    // TODO:  how to inform state ?
    // callback.addObserver(this);
    // callback.setOwner(std::make_unique<CharacterIchi>(*this));
}
} // namespace Ryu::Physics
