#pragma  once

#include <Ryu/Animation/AnimationData.h>
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
    // TODO: for some reason we have two presentationss of a FrameVector
    // (i guess this happened because of the animator uses a different datastructure -> someday we need to adjust ALL to one datastructure)
    void convertFramesDatastructure();

private:
    RyuParser::JsonParser jParser;
    std::map<Textures::LevelID, RyuParser::JsonAnimations> spritesheetConfigs;
};
