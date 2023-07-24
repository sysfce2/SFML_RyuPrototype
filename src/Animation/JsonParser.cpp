#include <Ryu/Animation/JsonParser.h>
#include <Ryu/Animation/EditorEnums.h>
#include <Ryu/Events/EventEnums.h>
#include <Ryu/Core/AssetIdentifiers.h>
#include <exception>
#include <fmt/core.h>
#include <fmt/format.h>
#include <string>
#include <typeinfo>
#include <ranges>
#include <string_view>
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

void splitStrings(std::string& s, char delimiter, std::vector<std::string>& output)
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

    void from_json(const json& j, Frame& frame) {
        j.at("duration").get_to(frame.duration);
        j.at("height").get_to(frame.height);
        j.at("width").get_to(frame.width);
        j.at("x_sheet").get_to(frame.x);
        j.at("y_sheet").get_to(frame.y);

        std::string frameEvent = j.at("event");
        Ryu::EEvent s = Ryu::EEvent::_from_string(frameEvent.c_str());
        frame.event = s;
    }

    void from_json(const json& j, Animation& ani) {
        j.at("Name").get_to(ani.name);
        j.at("Sheet_begin").get_to(ani.fromFrame);
        j.at("Sheet_end").get_to(ani.toFrame);
        j.at("AnimationDirection").get_to(ani.direction);
        j.at("numFrames").get_to(ani.numFrames);

        json frames = j["Frames"];

        sf::Vector2i frameSize;

        int i=0;
        for(auto frame : frames)
        {
            auto vecFrame = frame.get<Frame>();
            if(i==0){
                frameSize.x = vecFrame.height;
                frameSize.y = vecFrame.width;            
            }
            ++i;
            ani.frames.emplace_back(vecFrame);
        }

        ani.frameSize.x = frameSize.x;
        ani.frameSize.y = frameSize.y;

        // aniTime
        std::string timeDur;
        j.at("animationDuration").get_to(timeDur);

        std::vector<std::string> v;
        splitStrings(timeDur,' ', v);

        // atm we only support ms and s
        if(v.at(1) == "ms")
        {
            ani.animationDuration = sf::milliseconds((stoi(v.at(0))));
        }
        else if(v.at(1) == "s")
        {
            ani.animationDuration = sf::seconds((stoi(v.at(0))));
        }
        else { // how to handle other times ?
            throw std::exception();
        }

        j.at("repeat").get_to(ani.repeat);

        std::string aniId = j.at("AnimationId");
        std::string aniType = j.at("AnimationType");

        // TODO: this is ugly but it seems to work, its because animationId can hold dioffernet tyoes (variant)
        Textures::AnimationType aType = Textures::AnimationType::_from_string(aniType.c_str());

        try{
         switch(aType)
         {
            case Textures::AnimationType::Character:
            {
                Textures::CharacterID aId = Textures::CharacterID::_from_string(aniId.c_str());
                ani.animationId = aId;
                break;
            }
            case Textures::AnimationType::Scene:
            {
                Textures::SceneBGAni aId = Textures::SceneBGAni::_from_string(aniId.c_str());
                ani.animationId = aId;
                break;
            }
            default:
                throw std::exception();
         }

        }
        catch (std::exception())
        {
            fmt::print("Invalid AnimationId.");
        }

   }
    
    void from_json(const json& j, JsonAnimations& JsonAnimations) {
        j.at("Name").get_to(JsonAnimations.jsonName);
        std::string spritesheetId = j.at("Spritesheet");
        Textures::LevelID sId = Textures::LevelID::_from_string(spritesheetId.c_str());
        JsonAnimations.spritesheetId = sId;
        j.at("Path").get_to(JsonAnimations.spritesheetPath);
        json anis = j["Animations"];

        for(auto& ani : anis)
        {
            auto vecAni = ani.get<Animation>();
            JsonAnimations.animations.emplace(vecAni.animationId,vecAni);
        }
    }


JsonParser::JsonParser(){}

JsonParser::~JsonParser(){}


void
JsonParser::getAnimationsFromJson(json& jsonData, JsonAnimations& jsonAnis)
{
        try
        {
            // std::string jsonString = jsonData.dump();
            // fmt::print("JSON-OUTPUT: {}\n\n\n",jsonString);
            // convert json object to struct with animations
            jsonAnis = jsonData;
        }
        catch(const std::exception& e)
        {
            fmt::print("Error outputting json-content, missing Enum-value ?: {} \n",e.what());
        }
}

} /// namespace RyuParser
