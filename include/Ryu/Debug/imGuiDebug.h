#pragma once

#include <box2d/b2_math.h>
#include <fmt/core.h>
#include <imgui.h>
#include <iostream>

namespace RyuDebug {

// TODO: add the "states" to a heederfile (these are the only which are set
// somewhere else and used here
/*
static bool showImGuiDemoWindow;
static bool activateRyuDebug;
static const char *characterState;
static b2Vec2 charJumpForce;
static bool characterIsFalling;
static size_t numFrames;
static size_t numFramesVector;
static b2Vec2 jumpImpulseForward, jumpImpulseUp, massCenter;
static float charMass;

static float jumpImpulseForwardVec[4] = {.0f, .0f, .0f, .0f};
static float jumpImpulseUpVec[4] = {.0f, .0f, .0f, .0f};
static float massCenterVec[4] = {.0f, .0f, .0f, .0f};
*/
struct DebugData{
 bool showImGuiDemoWindow=false;
 bool activateRyuDebug=false;
 const char *characterState;
 b2Vec2 charJumpForce;
 bool characterIsFalling;
 size_t numFrames;
 size_t numFramesVector;
 b2Vec2 jumpImpulseForward, jumpImpulseUp, massCenter;
 float charMass;

 float jumpImpulseForwardVec[4] = {.0f, .0f, .0f, .0f};
 float jumpImpulseUpVec[4] = {.0f, .0f, .0f, .0f};
 float massCenterVec[4] = {.0f, .0f, .0f, .0f};

};

class DebugWidgets{

    public:
        DebugWidgets(){};

        void ShowWidgets();
        void CreateDebugGui();

        DebugData debugData;
};


} // namespace RyuDebug
