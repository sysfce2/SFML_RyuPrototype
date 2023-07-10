#pragma once

/*
    {
        "Name" : "Ichi",
        "Spritesheet" : "level1.png"
        "Animations" :
        [
            X {"name": "idle", 
            X "sheetBegin" : 0, X "sheetEnd" : 0, 
            X "frameSize" : {"width" : 80, "height" : 96},
            X "numFrames" : 4,
            X "duration_ms" : 800,
            X "frames" : 
                [
                    {"number" : 1, x "duration_ms" : 200, x "event" : "None", X height:96, X width: 80, X x_sheet: 0,X y:sheet: 0},
                    {"number" : 2, "duration_ms" : 120, "event" : "None"},
                    {"number" : 3, "duration_ms" : 80, "event" : "None"},
                    {"number" : 4, "duration_ms" : 400, "event" : "FootstepSFX"},
                ],
            X "repeat" : true,
            X "animationId" : "Textures::CharacterID::IchiIdleRun"        
            },
        ]
    }
*/
#include <Ryu/Animation/EditorEnums.h>
#include <nlohmann/json.hpp>
#include <map>

class EEvent;

using json = nlohmann::json;


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
      Textures::CharacterID animationId;
  
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
          ,animationId(Textures::CharacterID::None) {}
  
    };

    struct JsonAnimations {
        std::string jsonName;
        std::string spritesheetName;
        std::vector<Animation> animations;

        JsonAnimations()
            : jsonName(),
            spritesheetName(),
            animations({}) {}
    };

class JsonParser
{

    public:
        JsonParser();
        ~JsonParser();

        void getAnimationsFromJson(json& jsonData, JsonAnimations& jsonAnis);

    private:
        std::map<std::string,Animation> animations;
  
  
};

} /// RyuParser
