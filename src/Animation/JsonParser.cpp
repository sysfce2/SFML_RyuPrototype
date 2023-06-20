#include <Ryu/Animation/JsonParser.h>
#include <Ryu/Animation/EditorEnums.h>
#include <Ryu/Events/EventEnums.h>
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
    void from_json(const json& j, Frame& frame) {
        j.at("duration").get_to(frame.duration);
        j.at("height").get_to(frame.height);
        j.at("width").get_to(frame.width);
        j.at("x_sheet").get_to(frame.x);
        j.at("y_sheet").get_to(frame.y);

        std::string frameEvent = j.at("event");
        fmt::print("frameEvenent: {} \n",frameEvent);
        Ryu::EEvent s = Ryu::EEvent::_from_string(frameEvent.c_str());
        frame.event = s;
        // create EEvent from string ...
        // j.at("duration").get_to(frame.event);
    }    

    void from_json(const json& j, Textures::CharacterID& id){
        auto jString = j.dump(); 
        fmt::print("AniId {aniId}: ", jString);
        id = Textures::CharacterID::_from_string(jString.c_str());
        // .get<Textures::CharacterID>(ani.animationId);
    }

    void from_json(const json& j, Animation& ani) {
        j.at("Name").get_to(ani.name);
        j.at("Sheet_begin").get_to(ani.fromFrame);
        j.at("Sheet_end").get_to(ani.toFrame);
        j.at("AnimationDirection").get_to(ani.direction);
        j.at("numFrames").get_to(ani.numFrames);

        json frames = j["Frames"];

        for(auto frame : frames)
        {
            auto vecFrame = frame.get<Frame>();
            ani.frames.emplace_back(vecFrame);
        }

        // TODO: whats with Frames ? -> frames need to be serialized somehow else -> atm its an array of array ??? why:w
        // j.at("Frames").get_to(ani.frames);
        // json aniId = j.at("AnimationId");
        // ani.animationId = aniId.get<Textures::CharacterID>();
        //auto ID = aniId.get<Textures::CharacterID>();
        // fmt::print("AniId {aniId}: ", aniId.dump());
        // ani.animationId=Textures::CharacterID::_from_string((aniId.dump()).c_str());
        // write unittests to this to test if animation holds the correct stuff, so we can practise finally utests !
        // j.at("FrameSize").at("height").get_to(ani.frameSize.y);
        // j.at("FrameSize").at("width").get_to(ani.frameSize.x);
    
        // j.at("").get_to(ani.animationId);
        // j.at("").get_to(ani.animationId);
        // j.at("").get_to(ani.animationId);
    }
    
    void from_json(const json& j, JsonAnimations& JsonAnimations) {
        j.at("Name").get_to(JsonAnimations.jsonName);
        j.at("Spritesheet").get_to(JsonAnimations.spritesheetName);
        json anis = j["Animations"];

        for(auto ani : anis)
        {
            auto vecAni = ani.get<Animation>();
            JsonAnimations.animations.emplace_back(vecAni);
        }
    }


JsonParser::JsonParser(){}

JsonParser::~JsonParser(){}

void
JsonParser::unitTest()
{
    fmt::print("Connection to gtest works !");
}

void
JsonParser::getAnimationsFromJson(json& jsonData, JsonAnimations& jsonAnis)
{
        fmt::print("Parsing Json from file: \n");  
        std::string jsonString = jsonData.dump();
        try
        {
            fmt::print("JSON-OUTPUT: {}\n\n\n",jsonString);
            // convert json object to struct with animations
            jsonAnis = jsonData;
        }
        catch(const std::exception& e)
        {
            fmt::print("Error outputting json-content: {} \n",e.what());            
        }
}

} /// namespace RyuParser