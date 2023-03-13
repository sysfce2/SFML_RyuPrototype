#pragma once

#include <Ryu/Core/AssetManager.h> 
#include <Ryu/Core/AssetIdentifiers.h>
#include <Ryu/Animation/SpritesheetAnimation.h>

#include <bits/stdint-intn.h>
#include <cstdint>
#include <map>
#include <vector>
#include <string>

class SpritesheetAnimation;

using GuiCharTextureManager = AssetManager<sf::Texture, Textures::LevelID>;
using GuiTextureManager = AssetManager<sf::Texture, Textures::GuiID>;

namespace RyuAnimator
{

// fields important for every Animation 
// TODO (if Editor in use: delete struct in CharacterBase.h)
// it'll also included in AnimationSpec::Animation
struct AnimationConfig
{
    sf::Vector2i frameSize;
    sf::Vector2i startFrame;
    std::size_t numFrames;
    sf::Time duration;
    bool repeat;
    Textures::CharacterID animationId;
};

namespace AnimationSpec {
    // although the information for Frame & Taggednimation could be better consolidated
    // this is done this way bc the input json file-format from aseprite demands it
    // TODO: but we could use standard start values for other fields as well as we do with Event
    struct Frame 
    {
      int16_t duration;
      int16_t height;
      int16_t width;
      int16_t x; /// x-position in spritesheet
      int16_t y; /// y-position in spritesheet
      EEvent event;
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
    struct Spec{
      std::string specName;
      std::string spritesheetName;
      std::vector<Animation> animations; 
    }; 
  
} /// namespace AnimationSpec    

/*
    {
        "Name" : "Ichi",
        "Spritesheet" : "level1.png"
        "Animations" :
        [
            {"name": "idle", 
            "sheetStart" : {"x":1, "y":1}, 
            "frameSize" : {"width" : 80, "height" : 96},
            "numFrames" : 4,
            "duration_ms" : 800,
            "frames" : 
                [
                    {"number" : 1, "duration_ms" : 200, "event" : "None"},
                    {"number" : 2, "duration_ms" : 120, "event" : "None"},
                    {"number" : 3, "duration_ms" : 80, "event" : "None"},
                    {"number" : 4, "duration_ms" : 400, "event" : "FootstepSFX"},
                ],
            "repeat" : true,
            "animationId" : "Textures::CharacterID::IchiIdleRun"        
            },
        ]
    }
*/
using TaggedSheetAnimation = std::pair<std::string, std::vector<AnimationSpec::Animation>>  ; 

  class Editor
  {
    public:
      Editor();
      ~Editor();

    //private:

      void update(sf::Time dt);

      void initData();
  
      void createEditorWidgets(bool* p_open);

      void parseJsonData();

      bool showAnimationEditor;
  
      void exportAnimationDetailsToFile(char* JsonFilename);
      
      void setFrameDetails(int selectedAni, const TaggedSheetAnimation& sheet, int frameNumber);
      
      void setAnimationDuration(uint16_t duration, std::string aniName,std::string sheetName);
  
      void createAnimationDetails(int selectedAni,const TaggedSheetAnimation& sheet );
      void setSpritesheetAnimationDetails(const AnimationConfig& config);
      // map with all spritesheets loaded and according animations
      // key: spritesheetname, value: vector of animations
      std::map<std::string, std::vector<AnimationSpec::Animation> > animations;
      bool parsedSpritesheet;
      bool textureSet;
      std::string selectedSpritesheet;

  private:

      void setTooltipText(const char * tooltip);
      void initTextures();
      void editFrame(AnimationSpec::Animation ani, size_t frame);
      GuiCharTextureManager guiCharTextureManager;
      GuiTextureManager guiTextureManager;
      SpritesheetAnimation spritesheetAnimation;

      std::vector<AnimationSpec::Animation> animationSpecs;

      bool aniIsPlaying;
  };
}