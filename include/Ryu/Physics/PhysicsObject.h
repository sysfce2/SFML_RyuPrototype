#pragma once

#include <string>
#include<box2d/b2_math.h>

class b2Body;

struct PhysicsObject {
    std::string name;
    b2Body* pBody;

    PhysicsObject() : name(""), pBody(nullptr) {}

    PhysicsObject(std::string _name, b2Body* _pBody)
        : name(_name), pBody(_pBody) {}
};
