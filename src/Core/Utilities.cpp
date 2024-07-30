#include <Ryu/Core/Utilities.h>
#include <Ryu/Scene/SceneNode.h>
#include <Ryu/Physics/Raycast.h>

#include <box2d/box2d.h>

#include <string>
#include <map>
#include <memory>

/*
namespace RyuUtils
{
void
splitStrings(std::string& s, char delimiter, std::vector<std::string>& output)
{
    // sould work in c++20 but cant output wit fmt ...
    // auto splitStrings = std::string_view{timeDur} | std::ranges::views::split(' ');
    std::string current = "";
    for(int i = 0; i < s.size(); i++){
        if(s[i] == delimiter){
            if(current != ""){
                output.push_back(current);
                current = "";
            }
            continue;
        }
        current += s[i];
    }
    if(current.size() != 0)
        output.push_back(current);
}

}
*/

namespace RyuPhysics
{

}
