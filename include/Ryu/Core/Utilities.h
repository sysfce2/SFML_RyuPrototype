#pragma once

#include <Ryu/Control/CharacterEnums.h>
#include <Ryu/Physics/RaycastTypes.h>

#include <box2d/box2d.h>
#include <string>
#include <map>
#include <tuple>
#include <memory>

class b2World;
//class RayCastClosest;
/*
* box2D uses MSR - meter-seconds-radians - unities
*/
namespace Converter
{
    constexpr double PIXELS_PER_METERS = 32.0;
    constexpr double PI = 3.14159265358979323846;

    template<typename T>
    constexpr T pixelsToMeters(const T& x) {return x/PIXELS_PER_METERS;};

    template<typename T>
    constexpr T metersToPixels(const T& x) {return x*PIXELS_PER_METERS;};

    template<typename T>
    constexpr T degToRad(const T& x) {return PI*x/180.0;};

    template<typename T>
    constexpr T radToDeg(const T& x) {return 180*x/PI;};
}

//using RaycastPoints=std::map<RaycastPosition, std::pair<b2Vec2,b2Vec2> >;

namespace RyuPhysics
{

  //constexpr double raycastOffset = 25.0f;
  //RayCastClosest createRaycast(RaycastPosition positionLocal, std::pair<double,double> startPoint,float angle,float length, EMoveDirection charMoveDirection, std::unique_ptr<b2World>& physWorld, RaycastPoints& rayCastPoints);
  
}
/*
namespace RyuUtils
{
    void splitStrings(std::string& s, char delimiter, std::vector<std::string>& output);
}
*/
