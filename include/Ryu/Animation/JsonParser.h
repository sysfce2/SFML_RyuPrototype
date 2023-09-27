#pragma once

#include "Ryu/Core/AssetIdentifiers.h"
#include <Ryu/Animation/EditorEnums.h>
#include <nlohmann/json.hpp>
#include <map>

class EEvent;

using json = nlohmann::json;

using AnimationId = std::variant<Textures::CharacterID, Textures::SceneBGAni>;

namespace RyuParser {


    struct Frame 
    {
      int16_t duration;
      int16_t height;
      int16_t width;
      int16_t x; /// x-position in spritesheet
      int16_t y; /// y-position in spritesheet
      Ryu::EEvent event;

      Frame() :
        duration(0)
       ,height(0)
       ,width(0)
       ,x(0)
       ,y(0)
       ,event(Ryu::EEvent::None) {}     
    };

    struct Animation
    {

      std::string name;
      int16_t fromFrame; /// Frame Startposition in spritesheet
      int16_t toFrame; /// Frame Endposition in spritresheet
      std::string direction;
      std::vector<Frame> frames;
      std::size_t numFrames; // == TaggedAnimation::toFrame - TaggedAnimation::fromFrame
      sf::Vector2i frameSize;
      sf::Time animationDuration;
      bool repeat;
      Textures::AnimationType animationType;
      AnimationId animationId;
  
      Animation() : 
           name("name")
          ,fromFrame(0)
          ,toFrame(0)
          ,direction("forward")
          ,frames()
          ,numFrames(0)
          ,frameSize()
          ,animationDuration()
          ,repeat(false)
          ,animationType(Textures::AnimationType::None)
          ,animationId(Textures::CharacterID::None) {}
  
    };

    struct JsonAnimations {
        std::string jsonName;
        Textures::LevelID spritesheetId;
        std::string spritesheetPath;
        std::map<AnimationId,Animation> animations;

        JsonAnimations()
            : jsonName(),
              spritesheetId(Textures::LevelID::Unknown),
              spritesheetPath(),
              animations({}) {}
    };

class JsonParser
{

    public:
        JsonParser();
        ~JsonParser();
        void getAnimationsFromJson(json& jsonData, JsonAnimations& jsonAnis);
        void printJsonParserContent(RyuParser::JsonAnimations content);

    private:
        std::map<std::string,Animation> animations;
  
  
};

} /// RyuParser
