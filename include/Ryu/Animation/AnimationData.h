#pragma once

#include <Ryu/Animation/AnimationData.h>
#include <Ryu/Core/AssetIdentifiers.h>
#include <Ryu/Events/EventEnums.h>

#include <nlohmann/json.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

using AnimationId = std::variant<Textures::CharacterID, Textures::SceneBGAni>;

namespace RyuParser {

struct FrameBase
    {
      int16_t duration;
      int16_t height;
      int16_t width;
      int16_t x; /// x-position in spritesheet
      int16_t y; /// y-position in spritesheet
      Ryu::EEvent event;

      FrameBase() :
        duration(0)
       ,height(0)
       ,width(0)
       ,x(0)
       ,y(0)
       ,event(Ryu::EEvent::None) {}

};

  /*
  ** This is a kind of dirty workaround: nlohman needs to have the convertfunctions & according Datatsrutures
  ** to be in the same namespace. In the editor we load jsondata from the arttool (e.g. Aseprite),
  ** in the game we load jsondata from our own created json-file.
  ** But both use the same BaseDatastructure (Animation / Frame -structs)
   */
  struct Frame : FrameBase {};
  struct FrameEditor : FrameBase {};

  struct AnimationBase
    {

      std::string name;
      int16_t fromFrame; /// Frame Startposition in spritesheet
      int16_t toFrame; /// Frame Endposition in spritresheet
      std::string direction;
      std::vector<RyuParser::Frame> frames;
      std::size_t numFrames; // == TaggedAnimation::toFrame - TaggedAnimation::fromFrame
      sf::Vector2i frameSize;
      sf::Time animationDuration;
      bool repeat;
      Textures::AnimationType animationType;
      AnimationId animationId;
      sf::Vector2i positionCross;

      AnimationBase() :
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
          ,animationId(Textures::CharacterID::None)
          ,positionCross({0,0}) {}
  };

  struct Animation : AnimationBase {};
  struct AnimationEditor : AnimationBase {
    std::vector<RyuParser::FrameEditor> frames;
  };
} // RyuParser
