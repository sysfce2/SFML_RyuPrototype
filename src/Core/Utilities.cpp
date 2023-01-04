#include <Ryu/Core/Utilities.h>
#include <Ryu/Physics/Raycast.h>
#include <Ryu/Scene/SceneNode.h>

#include <box2d/box2d.h>

#include <string>
#include <map>
#include <memory>

namespace RyuPhysics
{
  
 void createRaycast(std::string type, std::pair<double,double> startPoint,float angle,float length, EMoveDirection charMoveDirection, std::unique_ptr<b2World>& physWorld, RaycastPoints& rayCastPoints)
  {
    // creating a raycast from the characters position downwards
    // 0° right / 90° up / 180° left / 270° down
    float raycastAngle = b2_pi * angle / 180.0f;
    float lengthMeter = Converter::pixelsToMeters<double>(length);
    b2Vec2 d(lengthMeter * cosf(raycastAngle),lengthMeter * sinf(raycastAngle));
    
    // direction according lookdir of character
    bool rcLeftOrRight = (angle >= 180 || angle == 0);
    int8 dir = ((charMoveDirection == EMoveDirection::Left && rcLeftOrRight) ? -1 : 1);

    b2Vec2 p1(Converter::pixelsToMeters<double>(startPoint.first),
              Converter::pixelsToMeters<double>(startPoint.second));

    b2Vec2 p2 = p1 + (dir * d); 
    auto rc = rayCastPoints.find(type);
    
    if(rc != rayCastPoints.end())
    {
        rc->second.first = p1;
        rc->second.second = p2; 
    }
    else
    {
        rayCastPoints.insert(std::make_pair(type,std::make_pair(p1,p2)));
    }
    
    RayCastClosest callback;
    physWorld.get()->RayCast(&callback, p1,p2);
    
    //TODO:  how to inform state ? 
    // callback.addObserver(this);
    //callback.setOwner(std::make_unique<CharacterIchi>(*this));
  }
}