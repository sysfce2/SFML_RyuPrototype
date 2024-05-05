#pragma once
#include <Thirdparty/betterEnum/enum.h>

namespace Ryu{

BETTER_ENUM(EEvent, char
        ,None=0
        ,CharacterSpeedChanged
        ,CharacterStartJump
        ,DebugToggle
        ,ImGuiDemoToggle
        ,RyuAnimatorToggle
        ,DebugValuesChanged
        ,TemporaryOutput
        ,TeleportWait
        ,TeleportDo
);

} // namespace Ryu
/*
NLOHMANN_JSON_SERIALIZE_ENUM( EEvent, {
        {None,"None"}
        ,{CharacterSpeedChanged,"CharacterSpeedChanged"
        ,{DebugToggle,"DebugToggle"}
        ,{ImGuiDemoToggle,"ImGuiDemoToggle"}
        ,{RyuAnimatorToggle,"RyuAnimatorToggle"}
              
})
*/
