#pragma once

#include <Ryu/Core/AssetManager.h> 
#include <Ryu/Core/AssetIdentifiers.h>
#include <Ryu/Animation/SpritesheetAnimation.h>

#include <map>
#include <vector>
#include <string>

class SpritesheetAnimation;

using GuiCharTextureManager = AssetManager<sf::Texture, Textures::LevelID>;

namespace RyuAnimator
{

namespace AnimationTags {
  struct Frame
  {
    int duration;
    int height;
    int width;
    int x; /// x-position in spritesheet
    int y; /// y-position in spritesheet
  };

  struct TaggedAnimation
  {
    std::string name;
    int fromFrame; /// Frame Startposition in spritesheet
    int toFrame; /// Frame Endposition in spritresheet
    std::string direction;
    std::vector<Frame> frames;
  };
} /// namespace AnimationTags

// fields important for every Animation 
// TODO (if Editor in use: delete struct in CharacterBase.h)
struct AnimationConfig
{
    sf::Vector2i frameSize;
    sf::Vector2i startFrame;
    std::size_t numFrames;
    sf::Time duration;
    bool repeat;
    Textures::CharacterID animationId;
};

using TaggedSheetAnimation = std::pair<std::string, std::vector<AnimationTags::TaggedAnimation>>  ; 

  class Editor
  {
    public:
      Editor();
      ~Editor();

    //private:

      void update(sf::Time dt);
  
      void createEditorWidgets(bool* p_open);

      void parseJsonData();

      bool showAnimationEditor;
  
      void createAnimationDetails(int selectedAni,const TaggedSheetAnimation& sheet );
      void setSpritesheetAnimationDetails(const AnimationConfig& config);
      // map with all spritesheets loaded and according animations
      // key: spritesheetname, value: vector of animations
      std::map<std::string, std::vector<AnimationTags::TaggedAnimation> > animations;
      bool parsedSpritesheet;
      bool textureSet;
      std::string selectedSpritesheet;

  private:
      void initTextures();
      GuiCharTextureManager guiCharTextureManager;
      SpritesheetAnimation spritesheetAnimation;

      bool aniIsPlaying;
  };
}