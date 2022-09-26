#pragma once
#include <functional>

class SceneNode;
namespace sf{
    class Time;
}

struct Command
{
    Command();
    std::function<void (SceneNode&, sf::Time)> action;
    unsigned int category; /// the recipient category of the command
};