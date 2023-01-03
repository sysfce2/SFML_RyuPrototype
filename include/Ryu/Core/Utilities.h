#pragma once

//#include <box2d/b2_math.h>
#include <Ryu/Control/CharacterEnums.h>
#include <Ryu/Physics/Raycast.h>

#include <box2d/box2d.h>
#include <string>
#include <map>

class b2World;

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

using RaycastPoints=std::map<std::string, std::pair<b2Vec2,b2Vec2> >;

namespace RyuPhysics
{
  void createRaycast(std::string type, std::pair<double,double> startPoint,float angle,float length, EMoveDirection charMoveDirection, std::unique_ptr<b2World>& physWorld, RaycastPoints& rayCastPoints);
  
}