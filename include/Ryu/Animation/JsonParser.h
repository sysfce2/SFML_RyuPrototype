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
using animation = RyuAnimator::AnimationSpec::Animation;

namespace RyuParser {




class JsonParser
{

    public:
        JsonParser();
        ~JsonParser();

        void getAnimationsFromJson(std::string jsonFile);

    private:
        std::map<std::string,animation> animations;
  
  
};

} /// RyuParser
