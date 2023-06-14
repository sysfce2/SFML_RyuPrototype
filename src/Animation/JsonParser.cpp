#include <Ryu/Animation/JsonParser.h>
#include <Ryu/Animation/EditorEnums.h>
#include <Ryu/Core/AssetIdentifiers.h>
#include <fmt/core.h>
#include <fmt/format.h>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>



    // struct Frame 
    // {
    //   int16_t duration;
    //   int16_t height;
    //   int16_t width;
    //   int16_t x; /// x-position in spritesheet
    //   int16_t y; /// y-position in spritesheet
    //   EEvent event;
    // };

    // struct Animation
    // {

    //   std::string name;
    //   int16_t fromFrame; /// Frame Startposition in spritesheet
    //   int16_t toFrame; /// Frame Endposition in spritresheet
    //   std::string direction;
    //   std::vector<Frame> frames;
    //   std::size_t numFrames; // == TaggedAnimation::toFrame - TaggedAnimation::fromFrame
    //   sf::Vector2i frameSize;
    //   sf::Time animationDuration;
    //   bool repeat;
    //   Textures::CharacterID animationId;
  


namespace RyuParser {
// namespace RyuAnimator::AnimationSpec {
    
    struct jsonFile {
        std::string jsonName;
        std::string spritesheetName;
        std::vector<Animation> animations;

        jsonFile()
            : jsonName(),
            spritesheetName(),
            animations({}) {}
    };

    void from_json(const json& j, Textures::CharacterID& id){
        auto jString = j.dump(); 
        id = Textures::CharacterID::_from_string(jString.c_str());
        // .get<Textures::CharacterID>(ani.animationId);
    }

    void from_json(const json& j, Animation& ani) {
        j.at("Name").get_to(ani.name);
        j.at("Sheet_begin").get_to(ani.fromFrame);
        j.at("Sheet_end").get_to(ani.toFrame);
        j.at("AnimationDirection").get_to(ani.direction);
        j.at("numFrames").get_to(ani.numFrames);

        // TODO: whats with Frames ? -> frames need to be serialized somehow else -> atm its an array of array ??? why:w
        // j.at("Frames").get_to(ani.frames);
        json aniId = j.at("AnimationId");
        // ani.animationId = aniId.get<Textures::CharacterID>();
        //auto ID = aniId.get<Textures::CharacterID>();
        ani.animationId=Textures::CharacterID::_from_string((aniId.dump()).c_str());
        // write unittests to this to test if animation holds the correct stuff, so we can practise finally utests !
        // j.at("FrameSize").at("height").get_to(ani.frameSize.y);
        // j.at("FrameSize").at("width").get_to(ani.frameSize.x);
    
        // j.at("").get_to(ani.animationId);
        // j.at("").get_to(ani.animationId);
        // j.at("").get_to(ani.animationId);
    }
    
    void from_json(const json& j, jsonFile& jsonFile) {
        j.at("Name").get_to(jsonFile.jsonName);
        j.at("Spritesheet").get_to(jsonFile.spritesheetName);
        json anis = j["Animations"];
        // std::vector<animation> anisVec;
// MAEH !!!!
        for(auto ani : anis)
        {
// using animation = RyuAnimator::AnimationSpec::Animation;
            auto vecAni = ani.get<Animation>();
            // animation vecAni = ani;
            jsonFile.animations.emplace_back(vecAni);
        }
        // j.at("Animations").get_to<std::vector<animation> >(jsonFile.animations);
    }


JsonParser::JsonParser(){}

JsonParser::~JsonParser(){}

void
JsonParser::unitTest()
{
    fmt::print("Connection to gtest works !");
}

void
JsonParser::getAnimationsFromJson(std::string jsonFile)
{
    std::ifstream f(jsonFile);
    fmt::print(stderr, "Open Json, {}!\n", jsonFile);
    try
    {
        json data = json::parse(f);

        
        
        fmt::print("Parsing Json from file: \n");  
        std::string jsonString = data.dump();
        try
        {
            fmt::print("JSON-OUTPUT: {}\n",jsonString);
        }
        catch(const std::exception& e)
        {
            fmt::print("Error outputting json-content: {} \n",e.what());            
        }
        // std::cout << jsonString << "\n";
    }
    catch(json::exception e)
    {
      fmt::print("{}: {}\n",e.id,e.what());
      fmt::print("Can't parse file, probably filestream-error. Filename correct ?\n");
    }
}

} /// namespace RyuParser