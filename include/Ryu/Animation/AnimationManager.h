#pragma  once

#include <Ryu/Animation/JsonParser.h>
#include <Ryu/Core/AssetIdentifiers.h>
#include <map>

class AnimationManager
{

public:
    void readAnimationConfigs();


private:
    std::map<Textures::LevelID, RyuParser::JsonAnimations> spritesheetConfigs;

};
