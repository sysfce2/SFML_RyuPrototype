#pragma once
#include <Thirdparty/betterEnum/enum.h>

BETTER_ENUM(EEvent, char
        ,None=0
        ,CharacterSpeedChanged
        ,DebugToggle
        ,ImGuiDemoToggle
        ,RyuAnimatorToggle
        
);
/*
NLOHMANN_JSON_SERIALIZE_ENUM( EEvent, {
        {None,"None"}
        ,{CharacterSpeedChanged,"CharacterSpeedChanged"
        ,{DebugToggle,"DebugToggle"}
        ,{ImGuiDemoToggle,"ImGuiDemoToggle"}
        ,{RyuAnimatorToggle,"RyuAnimatorToggle"}
              
})
*/