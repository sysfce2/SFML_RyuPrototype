#include <Ryu/Animation/AnimationData.h>
#include <Ryu/Animation/JsonParser.h>
#include <Ryu/Animation/EditorEnums.h>
#include <Ryu/Events/EventEnums.h>
#include <Ryu/Core/AssetIdentifiers.h>
//#include <Ryu/Core/Utilities.h>
#include <nlohmann/json.hpp>
#include <exception>
#include <fmt/core.h>
#include <fmt/format.h>
#include <string>
#include <typeinfo>
#include <ranges>
#include <string_view>
#include <fstream>
#include <iostream>
#include <variant>
#include <vector>

namespace RyuParser {
// namespace RyuAnimator::AnimationSpec {

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

    void from_json(const json& j, Frame& frame) {
        j.at("duration").get_to(frame.duration);
        j.at("height").get_to(frame.height);
        j.at("width").get_to(frame.width);
        j.at("x_sheet").get_to(frame.x);
        j.at("y_sheet").get_to(frame.y);

        std::string frameEvent = j.at("event");
        Ryu::EEvent s = Ryu::EEvent::_from_string(frameEvent.c_str());
        //fmt::print("FromJson(Event): {} \n",s._to_string());
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
        for(auto f : frames)
        {
            auto vecFrame = f.get<Frame>();
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
       //RyuUtils::splitStrings(timeDur,' ', v);
       splitStrings(timeDur,' ', v);

       // FIXME: as this should be a summary of duration of the frames it makes no sense to switch to s for now, duration in frames is always ms
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

        // set animationType
        ani.animationType = aType;

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

        json pivot = j["PivotNorm"];
        ani.pivot.x = pivot["x"];
        ani.pivot.y = pivot["y"];

   }
    
    void from_json(const json& j, JsonAnimations& JsonAnimations) {
        j.at("Name").get_to(JsonAnimations.jsonName);
        std::string spritesheetId = j.at("Spritesheet");
        std::string levelId = j.at("Level");
        Textures::LevelID lId = Textures::LevelID::_from_string(levelId.c_str());
        Textures::SpritesheetID sId = Textures::SpritesheetID::_from_string(spritesheetId.c_str());
        JsonAnimations.levelId = lId;
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
JsonParser::printJsonParserContent(RyuParser::JsonAnimations content)
{
    fmt::print("Parser holds these animations:\n");
    for(const auto& ani : content.animations)
    {
        fmt::print("Ani-Id: {} , Animation: {} \n",std::visit([](auto&& cId){return cId._to_string();},ani.first)
               , ani.second.name
                       );
    }
}


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
            fmt::print("Error outputting json-content, missing Enum-value or Json-value empty ?: {} \n",e.what());
        }
}

} /// namespace RyuParser
