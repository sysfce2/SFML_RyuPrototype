#pragma  once

#include <Ryu/Animation/JsonParser.h>
#include <Ryu/Core/AssetIdentifiers.h>
#include <filesystem>
#include <map>
#include <string_view>

const std::filesystem::path configPath{"assets/spritesheets/configs/"};
namespace fs = std::filesystem;

class AnimationManager
{

public:
    AnimationManager();
    ~AnimationManager();

    void readAnimationConfigs();
    std::string readFile(fs::path path);
    void outputConfigs();
    const RyuParser::Animation& getCharacterAnimationConfig(Textures::LevelID level, Textures::CharacterID aniId);

private:
    RyuParser::JsonParser jParser;
    std::map<Textures::LevelID, RyuParser::JsonAnimations> spritesheetConfigs;

};
