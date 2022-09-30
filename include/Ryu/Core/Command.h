#pragma once

#include <Ryu/Scene/SceneNode.h>
#include <functional>


namespace sf{
    class Time;
}

//namespace ryu{

struct Command
{
    Command();
    std::function<void (SceneNode&, sf::Time)> action;
    unsigned int category; /// the recipient category of the command
};

//} /// namespace ryu

/*
std::function<int(int)> increaser = std::bind(&add, _1, 1); // the first argument is forwarded and the second is fix (here 1)
int increased = increaser(5); // same as add(5, 1)
*/